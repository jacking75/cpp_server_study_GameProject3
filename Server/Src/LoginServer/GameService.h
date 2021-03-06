﻿#ifndef _GAME_SERVICE_H_
#define _GAME_SERVICE_H_
#include "LoginMsgHandler.h"

class CGameService  : public IPacketDispatcher
{
private:
	CGameService(void);
	virtual ~CGameService(void);

public:
	static CGameService* GetInstancePtr();

public:

	BOOL		Init();

	BOOL		Uninit();

	BOOL		Run();

	BOOL		OnNewConnect(CConnection* pConn);

	BOOL		OnCloseConnect(CConnection* pConn);

	BOOL		OnSecondTimer();

	BOOL		DispatchPacket( NetPacket* pNetPacket);

	BOOL        SendCmdToAccountConnection(UINT32 nMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata);

	BOOL		ConnectToAccountSvr();

	BOOL		ConnectToWatchServer();

	BOOL		SendWatchHeartBeat();

	BOOL		SetWatchIndex(UINT32 nIndex);
public:
	CLoginMsgHandler	m_LoginMsgHandler;
	UINT32              m_dwAccountConnID;
	UINT32				m_dwWatchSvrConnID;
	UINT32				m_dwWatchIndex;

public:
	//*********************消息处理定义开始******************************
	BOOL OnMsgWatchHeartBeatAck(NetPacket* pNetPacket);
	//*********************消息处理定义结束******************************
};

#endif
