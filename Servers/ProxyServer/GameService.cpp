#include "GameService.h"
//#include "Position.h"
//#include "../Message/Msg_Game.pb.h"
//#include "../Message/Msg_RetCode.pb.h"
//#include "../Message/Msg_ID.pb.h"

#include "..\..\ServerEngine\Log.h"
#include "..\..\ServerEngine\ConfigFile.h"
#include "..\..\ServerEngine\CommonFunc.h"
#include "..\..\ServerEngine\Connection.h"
#include "..\..\ServerEngine\ServiceBase.h"


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

bool CGameService::SetWatchIndex(uint32_t nIndex)
{
	m_dwWatchIndex = nIndex;

	return true;
}


bool CGameService::Init(ServerConfig config)
{
	ServerEngine::SetCurrentWorkDir("");

	if(!CLog::GetInstancePtr()->StartLog("ProxyServer", "log"))
	{
		return false;
	}

	CLog::GetInstancePtr()->LogError("---------서버 초기화-----------");

	auto pConfigFileInst = ServerEngine::ConfigFile::GetInstancePtr();

	/*if(!pConfigFileInst->Load("servercfg.ini"))
	{
		CLog::GetInstancePtr()->LogError("환경 설정 정보 로딩 실패!");
		return false;
	}*/

	CLog::GetInstancePtr()->SetLogLevel(config.LogLeve);

	UINT16 nPort = config.Port;
	INT32  nMaxConn = config.MaxConnCount;
	if(ServerEngine::ServiceBase::GetInstancePtr()->StartNetwork(nPort, nMaxConn, this) == false)
	{
		CLog::GetInstancePtr()->LogError("서비스 초기화 실패!");
		return false;
	}

	if (m_ProxyMsgHandler.Init(0) == false)
	{
		CLog::GetInstancePtr()->LogError("서비스 초기화 실패! - m_ProxyMsgHandler");
		return false;
	}
	
	CLog::GetInstancePtr()->LogError("---------서버 초기화 성공!--------");

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
	if (m_ProxyMsgHandler.DispatchPacket(pNetPacket))
	{
		return true;
	}

	return false;
}

uint32_t CGameService::GetLogicConnID()
{
	return m_dwLogicConnID;
}

bool CGameService::ConnectToLogicSvr()
{
	if (m_dwLogicConnID != 0)
	{
		return true;
	}
	
	//uint32_t nLogicPort = CConfigFile::GetInstancePtr()->GetIntValue("logic_svr_port");
	//std::string strLogicIp = CConfigFile::GetInstancePtr()->GetStringValue("logic_svr_ip");
	//CConnection* pConn = ServiceBase::GetInstancePtr()->ConnectTo(strLogicIp, nLogicPort);
	//ERROR_RETURN_FALSE(pConn != NULL);
	//m_dwLogicConnID = pConn->GetConnectionID();
	return true;
}

bool CGameService::Uninit()
{
	ServerEngine::ServiceBase::GetInstancePtr()->StopNetwork();	
	return true;
}

bool CGameService::Run()
{
	while(true)
	{
		ServerEngine::ServiceBase::GetInstancePtr()->Update();

		ServerEngine::Sleep(1);
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

