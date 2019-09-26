#pragma once

#include <Windows.h>

#include "..\..\ServerEngine\IBufferHandler.h"
#include "ProxyMsgHandler.h"

namespace ServerEngine
{
	class CConnection;
	struct NetPacket;
	class IDataBuffer;
}


class CGameService  : public ServerEngine::IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

	bool		Init();

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

	bool		SetWatchIndex(UINT32 nIndex);

public:
	CProxyMsgHandler	m_ProxyMsgHandler;

	UINT32				m_dwLogicConnID;

	UINT32				m_dwWatchSvrConnID;
	UINT32				m_dwWatchIndex;
public:
	//*********************消息处理定义开始******************************
	bool OnMsgWatchHeartBeatAck(ServerEngine::NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

