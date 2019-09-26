#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "..\..\ServerEngine\CommonFunc.h"
#include "..\..\ServerEngine\Connection.h"
#include "..\..\ServerEngine\ServiceBase.h"
#include "GameService.h"
//#include "Position.h"
//#include "../Message/Msg_Game.pb.h"
//#include "../Message/Msg_RetCode.pb.h"
//#include "../Message/Msg_ID.pb.h"

CGameService::CGameService(void)
{
	m_dwLogicConnID		= 0;
	m_dwWatchSvrConnID	= 0;
	m_dwWatchIndex		= 0;
}

CGameService::~CGameService(void)
{
}

CGameService* CGameService::GetInstancePtr()
{
	static CGameService _GameService;

	return &_GameService;
}

bool CGameService::SetWatchIndex(UINT32 nIndex)
{
	m_dwWatchIndex = nIndex;

	return TRUE;
}


bool CGameService::Init()
{
	ServerEngine::SetCurrentWorkDir("");

	/*if(!CLog::GetInstancePtr()->StartLog("ProxyServer", "log"))
	{
		return FALSE;
	}

	ServerEngine::CLog::GetInstancePtr()->LogError("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return FALSE;
	}

	ServerEngine::CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("proxy_log_level"));

	UINT16 nPort = ServerEngine::CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	INT32  nMaxConn = ServerEngine::CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return FALSE;
	}

	ERROR_RETURN_FALSE(m_ProxyMsgHandler.Init(0));

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");*/

	return TRUE;
}

bool CGameService::OnNewConnect(ServerEngine::CConnection* pConn)
{
	m_ProxyMsgHandler.OnNewConnect(pConn);

	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		SendWatchHeartBeat();
	}

	return TRUE;
}

bool CGameService::OnCloseConnect(ServerEngine::CConnection* pConn)
{
	if(pConn->GetConnectionID() == m_dwLogicConnID)
	{
		m_dwLogicConnID = 0;
	}

	m_ProxyMsgHandler.OnCloseConnect(pConn);

	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		m_dwWatchSvrConnID = 0;
	}

	return TRUE;
}

bool CGameService::OnSecondTimer()
{
	ConnectToLogicSvr();

	SendWatchHeartBeat();

	return TRUE;
}

bool CGameService::DispatchPacket(ServerEngine::NetPacket* pNetPacket)
{
	/*switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_ACK, OnMsgWatchHeartBeatAck)
	}

	if (m_ProxyMsgHandler.DispatchPacket(pNetPacket))
	{
		return TRUE;
	}*/

	return FALSE;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

bool CGameService::ConnectToLogicSvr()
{
	/*if (m_dwLogicConnID != 0)
	{
		return TRUE;
	}
	UINT32 nLogicPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectTo(strLogicIp, nLogicPort);
	ERROR_RETURN_FALSE(pConn != NULL);
	m_dwLogicConnID = pConn->GetConnectionID();*/
	return TRUE;
}

bool CGameService::Uninit()
{
	/*ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();*/

	return TRUE;
}

bool CGameService::Run()
{
	while(TRUE)
	{
		/*ServiceBase::GetInstancePtr()->Update();

		CommonFunc::Sleep(1);*/
	}

	return TRUE;
}

bool CGameService::SendWatchHeartBeat()
{
	/*if (m_dwWatchIndex == 0)
	{
		return TRUE;
	}

	if (m_dwWatchSvrConnID == 0)
	{
		ConnectToWatchServer();
		return TRUE;
	}

	WatchHeartBeatReq Req;
	Req.set_data(m_dwWatchIndex);
	Req.set_processid(CommonFunc::GetCurProcessID());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwWatchSvrConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Req);*/
	return TRUE;
}

bool CGameService::OnMsgWatchHeartBeatAck(ServerEngine::NetPacket* pNetPacket)
{
	/*WatchHeartBeatAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());*/

	return TRUE;
}


bool CGameService::ConnectToWatchServer()
{
	/*if (m_dwWatchSvrConnID != 0)
	{
		return TRUE;
	}
	UINT32 nWatchPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
	std::string strWatchIp = CConfigFile::GetInstancePtr()->GetStringValue("watch_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strWatchIp, nWatchPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwWatchSvrConnID = pConnection->GetConnectionID();*/
	return TRUE;
}

