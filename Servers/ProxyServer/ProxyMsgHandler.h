#pragma once

#ifdef _MSC_BUILD
#else
#include "..\..\ServerEngine\Platform.h"
#endif

#include <cstdint>
#include <map>


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

	bool		Init(uint32_t dwReserved);

	bool		Uninit();

	bool		DispatchPacket(ServerEngine::NetPacket* pNetPacket);

	bool		OnNewConnect(ServerEngine::CConnection* pConn);

	bool		OnCloseConnect(ServerEngine::CConnection* pConn);
public:
	bool		RelayToGameServer(CProxyPlayer* pClientObj, ServerEngine::IDataBuffer* pBuffer);

	bool		RelayToLogicServer(ServerEngine::IDataBuffer* pBuffer);

	bool        RelayToConnect(uint32_t dwConnID, ServerEngine::IDataBuffer* pBuffer);

	uint32_t      GetGameSvrConnID(uint32_t dwSvrID);

	bool		IsServerConnID(uint32_t dwConnID);
	//*********************消息处理定义开始******************************
public:
	bool		OnMsgGameSvrRegister(ServerEngine::NetPacket* pPacket);
	bool		OnMsgEnterSceneReq(ServerEngine::NetPacket* pPacket);
	bool		OnMsgBroadMessageNty(ServerEngine::NetPacket* pPacket);
	bool		OnMsgRoleLoginAck(ServerEngine::NetPacket* pPacket);
	bool		OnMsgRoleLogoutReq(ServerEngine::NetPacket* pPacket);
	bool		OnMsgKickoutNty(ServerEngine::NetPacket* pPacket);
public:
	std::map<uint32_t, uint32_t> m_mapSvrIDtoConnID;

};

