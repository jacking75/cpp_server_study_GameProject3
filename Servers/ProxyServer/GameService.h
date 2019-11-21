#pragma once

#include "ProxyMsgHandler.h"

#include "..\..\ServerEngine\IBufferHandler.h"

#include <cstdint>

namespace ServerEngine
{
	class CConnection;
	struct NetPacket;
	class IDataBuffer;
}

struct ServerConfig
{
	int LogLeve = 1;
	int Index = 1;
	unsigned int MaxConnCount = 0;
	unsigned short Port = 0;
};

class CGameService  : public ServerEngine::IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	bool		Init(ServerConfig config);

	bool		Uninit();

	bool		Run();

	bool		OnNewConnect(ServerEngine::CConnection* pConn);

	bool		OnCloseConnect(ServerEngine::CConnection* pConn);

	bool		OnSecondTimer();

	bool		DispatchPacket(ServerEngine::NetPacket* pNetPacket);

	UINT32		GetLogicConnID();

	bool		ConnectToLogicSvr();

	bool		ConnectToWatchServer();

	bool		SendWatchHeartBeat();

	bool		SetWatchIndex(uint32_t nIndex);

public:
	CProxyMsgHandler	m_ProxyMsgHandler;

	uint32_t				m_dwLogicConnID;

	uint32_t				m_dwWatchSvrConnID;
	uint32_t				m_dwWatchIndex;
public:
	//*********************메시지 처리 함수 정의 시작**************************
	bool OnMsgWatchHeartBeatAck(ServerEngine::NetPacket* pNetPacket);
	//*********************메시지 처리 함수 정의 끝****************************
};

