#pragma once

#include <map>



#ifdef _MSC_BUILD
#else
#include "..\..\ServerEngine\Platform.h"
#endif


namespace ServerEngine
{
	class CConnection;
	struct NetPacket;
	class IDataBuffer;
}

class  CProxyPlayer;

class CProxyMsgHandler
{
public:
	CProxyMsgHandler();

	~CProxyMsgHandler();

	bool		Init(UINT32 dwReserved);

	bool		Uninit();

	bool		DispatchPacket(ServerEngine::NetPacket* pNetPacket);

	bool		OnNewConnect(ServerEngine::CConnection* pConn);

	bool		OnCloseConnect(ServerEngine::CConnection* pConn);
public:
	bool		RelayToGameServer(CProxyPlayer* pClientObj, ServerEngine::IDataBuffer* pBuffer);

	bool		RelayToLogicServer(ServerEngine::IDataBuffer* pBuffer);

	bool        RelayToConnect(UINT32 dwConnID, ServerEngine::IDataBuffer* pBuffer);

	UINT32      GetGameSvrConnID(UINT32 dwSvrID);

	bool		IsServerConnID(UINT32 dwConnID);
	//*********************消息处理定义开始******************************
public:
	bool		OnMsgGameSvrRegister(ServerEngine::NetPacket* pPacket);
	bool		OnMsgEnterSceneReq(ServerEngine::NetPacket* pPacket);
	bool		OnMsgBroadMessageNty(ServerEngine::NetPacket* pPacket);
	bool		OnMsgRoleLoginAck(ServerEngine::NetPacket* pPacket);
	bool		OnMsgRoleLogoutReq(ServerEngine::NetPacket* pPacket);
	bool		OnMsgKickoutNty(ServerEngine::NetPacket* pPacket);
public:
	std::map<UINT32, UINT32> m_mapSvrIDtoConnID;

};

