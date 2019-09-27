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

	return true;
}


bool CGameService::Init()
{
	ServerEngine::SetCurrentWorkDir("");

	/*if(!CLog::GetInstancePtr()->StartLog("ProxyServer", "log"))
	{
		return false;
	}

	ServerEngine::CLog::GetInstancePtr()->LogError("---------服务器开始启动-----------");

	if(!CConfigFile::GetInstancePtr()->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("配制文件加载失败!");
		return false;
	}

	ServerEngine::CLog::GetInstancePtr()->SetLogLevel(CConfigFile::GetInstancePtr()->GetIntValue("proxy_log_level"));

	UINT16 nPort = ServerEngine::CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_port");
	INT32  nMaxConn = ServerEngine::CConfigFile::GetInstancePtr()->GetIntValue("proxy_svr_max_con");
	if(!ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this))
	{
		CLog::GetInstancePtr()->LogError("启动服务失败!");
		return false;
	}

	ERROR_RETURN_false(m_ProxyMsgHandler.Init(0));

	CLog::GetInstancePtr()->LogError("---------服务器启动成功!--------");*/

	return true;
}

bool CGameService::OnNewConnect(ServerEngine::CConnection* pConn)
{
	m_ProxyMsgHandler.OnNewConnect(pConn);

	if (pConn->GetConnectionID() == m_dwWatchSvrConnID)
	{
		SendWatchHeartBeat();
	}

	return true;
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

	return true;
}

bool CGameService::OnSecondTimer()
{
	ConnectToLogicSvr();

	SendWatchHeartBeat();

	return true;
}

bool CGameService::DispatchPacket(ServerEngine::NetPacket* pNetPacket)
{
	/*switch(pNetPacket->m_dwMsgID)
	{
			PROCESS_MESSAGE_ITEM(MSG_WATCH_HEART_BEAT_ACK, OnMsgWatchHeartBeatAck)
	}

	if (m_ProxyMsgHandler.DispatchPacket(pNetPacket))
	{
		return true;
	}*/

	return false;
}

UINT32 CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

bool CGameService::ConnectToLogicSvr()
{
	/*if (m_dwLogicConnID != 0)
	{
		return true;
	}
	UINT32 nLogicPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectTo(strLogicIp, nLogicPort);
	ERROR_RETURN_FALSE(pConn != NULL);
	m_dwLogicConnID = pConn->GetConnectionID();*/
	return true;
}

bool CGameService::Uninit()
{
	/*ServiceBase::GetInstancePtr()->StopNetwork();
	google::protobuf::ShutdownProtobufLibrary();*/

	return true;
}

bool CGameService::Run()
{
	while(true)
	{
		/*ServiceBase::GetInstancePtr()->Update();

		CommonFunc::Sleep(1);*/
	}

	return true;
}

bool CGameService::SendWatchHeartBeat()
{
	/*if (m_dwWatchIndex == 0)
	{
		return true;
	}

	if (m_dwWatchSvrConnID == 0)
	{
		ConnectToWatchServer();
		return true;
	}

	WatchHeartBeatReq Req;
	Req.set_data(m_dwWatchIndex);
	Req.set_processid(CommonFunc::GetCurProcessID());
	ServiceBase::GetInstancePtr()->SendMsgProtoBuf(m_dwWatchSvrConnID, MSG_WATCH_HEART_BEAT_REQ, 0, 0, Req);*/
	return true;
}

bool CGameService::OnMsgWatchHeartBeatAck(ServerEngine::NetPacket* pNetPacket)
{
	/*WatchHeartBeatAck Ack;
	Ack.ParsePartialFromArray(pNetPacket->m_pDataBuffer->GetData(), pNetPacket->m_pDataBuffer->GetBodyLenth());*/

	return true;
}


bool CGameService::ConnectToWatchServer()
{
	/*if (m_dwWatchSvrConnID != 0)
	{
		return true;
	}
	UINT32 nWatchPort = CConfigFile::GetInstancePtr()->GetIntValue("watch_svr_port");
	std::string strWatchIp = CConfigFile::GetInstancePtr()->GetStringValue("watch_svr_ip");
	CConnection* pConnection = ServiceBase::GetInstancePtr()->ConnectTo(strWatchIp, nWatchPort);
	ERROR_RETURN_FALSE(pConnection != NULL);
	m_dwWatchSvrConnID = pConnection->GetConnectionID();*/
	return true;
}

