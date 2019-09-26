﻿#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "..\..\ServerEngine\Log.h"
#include "..\..\ServerEngine\PacketHeader.h"
#include "..\..\ServerEngine\IBufferHandler.h"
#include "..\..\ServerEngine\CommandDef.h"
#include "..\..\ServerEngine\Connection.h"
#include "..\..\ServerEngine\ServiceBase.h"
#include "ProxyMsgHandler.h"
#include "GameService.h"
#include "ProxyPlayerMgr.h"

#include "../Common/PacketID.h"
//#include "../Message/Msg_ID.pb.h"
//#include "../Message/Msg_RetCode.pb.h"
//#include "../Message/Msg_Game.pb.h"



CProxyMsgHandler::CProxyMsgHandler()
{
	CProxyPlayerMgr::GetInstancePtr();
}

CProxyMsgHandler::~CProxyMsgHandler()
{

}

bool CProxyMsgHandler::Init(UINT32 dwReserved)
{
	return TRUE;
}

bool CProxyMsgHandler::Uninit()
{
	return TRUE;
}

bool CProxyMsgHandler::DispatchPacket(ServerEngine::NetPacket* pNetPacket)
{
	auto pPacketHeader = (ServerEngine::PacketHeader*)pNetPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_FALSE(pPacketHeader != NULL);

	switch(pNetPacket->m_dwMsgID)
	{
			/*PROCESS_MESSAGE_ITEM(MSG_GASVR_REGTO_PROXY_REQ,			OnMsgGameSvrRegister);
			PROCESS_MESSAGE_ITEM(MSG_BROAD_MESSAGE_NOTIFY,			OnMsgBroadMessageNty);
			PROCESS_MESSAGE_ITEM(MSG_ENTER_SCENE_REQ,				OnMsgEnterSceneReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGIN_ACK,				OnMsgRoleLoginAck);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_LOGOUT_REQ,				OnMsgRoleLogoutReq);
			PROCESS_MESSAGE_ITEM(MSG_ROLE_OTHER_LOGIN_NTY,			OnMsgKickoutNty);*/

		case MSG_ROLE_LIST_REQ:
		case MSG_ROLE_CREATE_REQ:
		case MSG_ROLE_DELETE_REQ:
		case MSG_ROLE_LOGIN_REQ:


		case MSG_ROLE_RECONNECT_REQ:
		{
			//因为此时逻辑服还没有对象，需要告诉逻辑服，当前的客户端连接ID
			pPacketHeader->dwUserData = pNetPacket->m_dwConnID;
			RelayToLogicServer(pNetPacket->m_pDataBuffer);
		}
		break;
		default:
		{
			//if((pPacketHeader->dwMsgID >= MSG_LOGICSVR_MSGID_BEGIN) && (pPacketHeader->dwMsgID <= MSG_LOGICSVR_MSGID_END))
			//{
			//	if(pNetPacket->m_dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
			//	{
			//		RelayToConnect(pPacketHeader->dwUserData, pNetPacket->m_pDataBuffer);
			//	}
			//	else //这是客户端发过来的消息
			//	{
			//		RelayToLogicServer(pNetPacket->m_pDataBuffer);
			//	}
			//}
			//else if((pPacketHeader->dwMsgID >= MSG_SCENESVR_MSGID_BEGIN) && (pPacketHeader->dwMsgID <= MSG_SCENESVR_MSGID_END))
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
			//}
		}
	}

	return TRUE;
}

bool CProxyMsgHandler::OnNewConnect(ServerEngine::CConnection* pConn)
{
	return TRUE;
}

bool CProxyMsgHandler::OnCloseConnect(ServerEngine::CConnection* pConn)
{
	if(pConn->GetConnectionData() == 0)
	{
		return TRUE;
	}

	/*RoleDisconnectReq Req;
	Req.set_roleid(pConn->GetConnectionData());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(CGameService::GetInstancePtr()->GetLogicConnID(), MSG_DISCONNECT_NTY, pConn->GetConnectionData(), 0,  Req);

	CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pConn->GetConnectionData());
	ERROR_RETURN_TRUE(pPlayer != NULL);

	UINT32 dwConnID = GetGameSvrConnID(pPlayer->GetGameSvrID());
	ERROR_RETURN_TRUE(dwConnID != 0);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(dwConnID, MSG_DISCONNECT_NTY, pPlayer->GetCharID(), pPlayer->GetCopyGuid(),  Req);*/
	return TRUE;
}

bool CProxyMsgHandler::RelayToGameServer( CProxyPlayer* pClientObj, ServerEngine::IDataBuffer* pBuffer )
{
	ERROR_RETURN_FALSE(pClientObj != NULL);

	return TRUE;
}

bool CProxyMsgHandler::RelayToLogicServer(ServerEngine::IDataBuffer* pBuffer )
{
	if(!ServerEngine::ServiceBase::GetInstancePtr()->SendMsgBuffer(CGameService::GetInstancePtr()->GetLogicConnID(), pBuffer))
	{
		return FALSE;
	}

	return TRUE;
}

bool CProxyMsgHandler::RelayToConnect(UINT32 dwConnID, ServerEngine::IDataBuffer* pBuffer)
{
	if(!ServerEngine::ServiceBase::GetInstancePtr()->SendMsgBuffer(dwConnID, pBuffer))
	{
		return FALSE;
	}

	return TRUE;
}

UINT32 CProxyMsgHandler::GetGameSvrConnID(UINT32 dwSvrID)
{
	std::map<UINT32, UINT32>::iterator itor = m_mapSvrIDtoConnID.find(dwSvrID);
	if(itor != m_mapSvrIDtoConnID.end())
	{
		return itor->second;
	}

	return 0;
}

bool CProxyMsgHandler::IsServerConnID(UINT32 dwConnID)
{
	if(dwConnID == CGameService::GetInstancePtr()->GetLogicConnID())
	{
		return TRUE;
	}

	for(std::map<UINT32, UINT32>::iterator itor = m_mapSvrIDtoConnID.begin(); itor != m_mapSvrIDtoConnID.end(); itor++)
	{
		if(itor->second == dwConnID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

bool CProxyMsgHandler::OnMsgGameSvrRegister(ServerEngine::NetPacket* pPacket)
{
	/*SvrRegToSvrReq Req;
	Req.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());

	m_mapSvrIDtoConnID.insert(std::make_pair(Req.serverid(), pPacket->m_dwConnID));

	SvrRegToSvrAck Ack;
	Ack.set_retcode(MRC_SUCCESSED);
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(pPacket->m_dwConnID, MSG_GASVR_REGTO_PROXY_ACK, 0, 0, Ack);*/

	return TRUE;
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
	return TRUE;
}

bool CProxyMsgHandler::OnMsgBroadMessageNty(ServerEngine::NetPacket* pPacket)
{
	/*BroadMessageNotify Nty;
	Nty.ParsePartialFromArray(pPacket->m_pDataBuffer->GetData(), pPacket->m_pDataBuffer->GetBodyLenth());
	PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();

	for(int i = 0; i < Nty.connid_size(); i++)
	{
		ServiceBase::GetInstancePtr()->SendMsgRawData(Nty.connid(i), Nty.msgid(), 0, 0, Nty.msgdata().c_str(), (UINT32)Nty.msgdata().size());
	}*/

	return TRUE;
}

bool CProxyMsgHandler::OnMsgRoleLoginAck(ServerEngine::NetPacket* pPacket)
{
	/*PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);
	CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
	ERROR_RETURN_TRUE(pConnection != NULL);
	pConnection->SetConnectionData(pPacketHeader->u64TargetID);
	RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);

	CProxyPlayer* pPlayer = CProxyPlayerMgr::GetInstancePtr()->GetByCharID(pPacketHeader->u64TargetID);
	if (pPlayer == NULL)
	{
		pPlayer = CProxyPlayerMgr::GetInstancePtr()->CreateProxyPlayer(pPacketHeader->u64TargetID);
	}

	pPlayer->SetConnID(pPacketHeader->dwUserData);*/

	return TRUE;
}

bool CProxyMsgHandler::OnMsgRoleLogoutReq(ServerEngine::NetPacket* pPacket)
{
	/*PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	RelayToLogicServer(pPacket->m_pDataBuffer);

	CConnection* pConnection = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
	ERROR_RETURN_TRUE(pConnection != NULL);
	pConnection->SetConnectionData(0);*/
	return TRUE;
}

bool CProxyMsgHandler::OnMsgKickoutNty(ServerEngine::NetPacket* pPacket)
{
	/*PacketHeader* pPacketHeader = (PacketHeader*)pPacket->m_pDataBuffer->GetBuffer();
	ERROR_RETURN_TRUE(pPacketHeader != NULL);

	RelayToConnect(pPacketHeader->dwUserData, pPacket->m_pDataBuffer);
	CConnection* pConn = ServiceBase::GetInstancePtr()->GetConnectionByID(pPacketHeader->dwUserData);
	if (pConn != NULL)
	{
		pConn->SetConnectionData(0);
	}*/

	return TRUE;
}
