#include "ProxyMsgHandler.h"
#include "GameService.h"
#include "ProxyPlayerMgr.h"

#include "..\..\ServerEngine\Log.h"
#include "..\..\ServerEngine\PacketHeader.h"
#include "..\..\ServerEngine\IBufferHandler.h"
#include "..\..\ServerEngine\CommandDef.h"
#include "..\..\ServerEngine\Connection.h"
#include "..\..\ServerEngine\ServiceBase.h"
#include "..\..\ServerEngine\CheckMacroDefine.h"

#include "../Common/PacketID.h"


ServerEngine::ServiceBase* serviceInst = nullptr;

CProxyMsgHandler::CProxyMsgHandler()
{
	CProxyPlayerMgr::GetInstancePtr();
}

CProxyMsgHandler::~CProxyMsgHandler()
{

}

bool CProxyMsgHandler::Init(uint32_t dwReserved)
{
	serviceInst = ServerEngine::ServiceBase::GetInstancePtr();
	return true;
}

bool CProxyMsgHandler::Uninit()
{
	return true;
}

bool CProxyMsgHandler::DispatchPacket(ServerEngine::NetPacket* pNetPacket)
{
	auto pPacketHeader = (ServerEngine::PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_FALSE(pPacketHeader != NULL);

	switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_GASVR_REGTO_PROXY_REQ,			OnMsgGameSvrRegister);
			PROCESS_MESSAGE_ITEM(MSG_BROAD_MESSAGE_NOTIFY,			OnMsgBroadMessageNty);
			PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,				OnMsgEnterSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,				OnMsgRoleLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,				OnMsgRoleLogoutReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_OTHER_LOGIN_NTY,			OnMsgKickoutNty);

		case MSG_ROLE_LIST_REQ:
		case MSG_ROLE_CREATE_REQ:
		case MSG_ROLE_DELETE_REQ:
		case MSG_ROLE_LOGIN_REQ:
		case MSG_ROLE_RECONNECT_REQ:
		{
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;
			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
		default:
		{
			// 클라이언트 혹은 로직 서버로 보낸다
			/*if((pPacketHeader->dwMsgID >= MSG_LOGICSVR_MSGID_BEGIN) && (pPacketHeader->dwMsgID <= MSG_LOGICSVR_MSGID_END))
			{
				if(pNetPacket->m_dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
				{
					RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
				}
				else 
				{
					RelayToLogicServer(pNetPacket->m_pDataBuffer);
				}

				return true;
			}*/
			
			//씬 서버로 보낸다
			//if((pPacketHeader->dwMsgID >= MSG_SCENESVR_MSGID_BEGIN) && (pPacketHeader->dwMsgID <= MSG_SCENESVR_MSGID_END))
			//{
			//	if(IsServerConnID(pNetPacket->m_dwConnID))
			//	{
			//		RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
			//	}
			//	else //这是客户端发过来的消息
			//	{
			//		CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pPacketHeader->u64TargetID);
			//		ServerEngine::ERROR_RETURN_TRUE(pPlayer != NULL);

			//		auto dwConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
			//		ServerEngine::ERROR_RETURN_TRUE(dwConnID != 0);

			//		RelayToConnect(dwConnID, pNetPacket->m_pDataBuffer);
			//	}

			//	return true;
			//}
		}
	}

	return true;
}

bool CProxyMsgHandler::OnNewConnect(ServerEngine::CConnection* pConn)
{
	return true;
}

bool CProxyMsgHandler::OnCloseConnect(ServerEngine::CConnection* pConn)
{
	if(pConn->GetConnectionData() == 0)
	{
		return true;
	}

	// 이 클라이언트가 속해 있는 서버에 통보한다
	/*
	RoleDisconnectReq Req;
	Req.set_roleid(pConn->GetConnectionData());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pConn->GetConnectionData(), 0,  Req);

	CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pConn->GetConnectionData());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	UINT32 dwConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
	ERROR_RETURN_TRUE(dwConnID != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_DISCONNECT_NTY, pPlayer->GetCharID(), pPlayer->GetCopyGuid(),  Req);
	*/
	return true;
}

bool CProxyMsgHandler::RelayToGameServer( CProxyPlayer* pClientObj, ServerEngine::IDataBuffer* pBuffer )
{
	ERROR_RETURN_FALSE(pClientObj != NULL);

	return true;
}

bool CProxyMsgHandler::RelayToLogicServer(ServerEngine::IDataBuffer* pBuffer )
{
	if(!ServerEngine::ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetLogicConnID(), pBuffer))
	{
		return false;
	}

	return true;
}

bool CProxyMsgHandler::RelayToConnect(uint32_t dwConnID, ServerEngine::IDataBuffer* pBuffer)
{
	if(!ServerEngine::ServiceBase::GetInstancePtr()->SendMsgBuffer(dwConnID, pBuffer))
	{
		return false;
	}

	return true;
}

uint32_t CProxyMsgHandler::GetGameSvrConnID(uint32_t dwSvrID)
{
	std::map<UINT32, UINT32>::iterator itor = m_mapSvrIDtoConnID.find(dwSvrID);
	if(itor != m_mapSvrIDtoConnID.end())
	{
		return itor->second;
	}

	return 0;
}

bool CProxyMsgHandler::IsServerConnID(uint32_t dwConnID)
{
	if(dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
	{
		return true;
	}

	for(auto itor = m_mapSvrIDtoConnID.begin(); itor != m_mapSvrIDtoConnID.end(); itor++)
	{
		if(itor->second == dwConnID)
		{
			return true;
		}
	}

	return false;
}

bool CProxyMsgHandler::OnMsgGameSvrRegister(ServerEngine::NetPacket* pPacket)
{
	/*SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	m_mapSvrIDtoConnID.insert(std::make_pair(Req.serverid(), pPacket->m_dwConnID));

	SvrRegToSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_GASVR_REGTO_PROXY_ACK, 0, 0, Ack);*/

	return true;
}

bool CProxyMsgHandler::OnMsgEnterSceneReq(ServerEngine::NetPacket* pNetPacket)
{
	/*EnterSceneReq Req;
	Req.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pPacketHeader = (PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader->u64TargetID != 0);

	CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(Req.roleid());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	pPlayer->SetGameSvrInfo(Req.serverid(), Req.copyguid());

	UINT32 dwConnID = GetGameSvrConnID(Req.serverid());
	ERROR_RETURN_TRUE(dwConnID != 0)
	pPacketHeader->u64TargetID = pNetPacket->m_dwConnID;
	RelayToConnect(dwConnID, pNetPacket->m_pDataBuffer);
	RelayToLogicServer(pNetPacket->m_pDataBuffer);*/
	return true;
}

bool CProxyMsgHandler::OnMsgBroadMessageNty(ServerEngine::NetPacket* pPacket)
{
	/*BroadMessageNotify Nty;
	Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	auto pPacketHeader = (ServerEngine::PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	for(int i = 0; i < Nty.connid_size(); i++)
	{
		serviceInst->SendMsgRawData(Nty.connid(i), Nty.msgid(), 0, 0, Nty.msgdata().c_str(), (UINT32)Nty.msgdata().size());
	}*/

	return true;
}

bool CProxyMsgHandler::OnMsgRoleLoginAck(ServerEngine::NetPacket* pPacket)
{
	auto pPacketHeader = (ServerEngine::PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	auto pConnection = serviceInst->GetConnectionByID(pPacketHeader->dwUserData);
	ERROR_RETURN_TRUE(pConnection != NULL);

	pConnection->SetConnectionData(pPacketHeader->u64TargetID);
	RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);

	auto pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pPacketHeader->u64TargetID);
	if (pPlayer == NULL)
	{
		pPlayer = CProxyPlayerMgr::GetInstancePtr()->CreateProxyPlayer(pPacketHeader->u64TargetID);
	}

	pPlayer->SetConnID(pPacketHeader->dwUserData);

	return true;
}

bool CProxyMsgHandler::OnMsgRoleLogoutReq(ServerEngine::NetPacket* pPacket)
{
	auto pPacketHeader = (ServerEngine::PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	RelayToLogicServer(pPacket->m_pDataBuffer);

	auto pConnection = serviceInst->GetConnectionByID(pPacketHeader->dwUserData);
	ERROR_RETURN_TRUE(pConnection != NULL);
	pConnection->SetConnectionData(0);
	return true;
}

bool CProxyMsgHandler::OnMsgKickoutNty(ServerEngine::NetPacket* pPacket)
{
	auto pPacketHeader = (ServerEngine::PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);
	auto pConn = serviceInst->GetConnectionByID(pPacketHeader->dwUserData);
	if (pConn != NULL)
	{
		pConn->SetConnectionData(0);
	}

	return true;
}
