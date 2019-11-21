#pragma once

#ifndef _MSC_BUILD
#include "..\..\ServerEngine\Platform.h"
#endif

#include <cstdint>
#include <unordered_map>

class  CProxyPlayer
{
public:
	CProxyPlayer(): m_u64RoleID(0), m_dwGameSvrID(0) {}

	~CProxyPlayer() {}

public:
	uint32_t	GetGameSvrID() { return m_dwGameSvrID; }

	void	SetGameSvrInfo(uint32_t dwSvrID, uint32_t dwCopyGuid);

	void	SetConnID(uint32_t dwConnID);

	uint64_t	GetCharID() { return m_u64RoleID; }

	uint32_t  GetConnID() { return m_dwConnID; }

	uint32_t  GetCopyGuid() { return m_dwCopyGuid; }

public:
	uint64_t	m_u64RoleID;

	uint32_t	m_dwGameSvrID;

	uint32_t  m_dwConnID;

	uint32_t  m_dwCopyGuid;
};

class CProxyPlayerMgr
{
public:
	static CProxyPlayerMgr* GetInstancePtr();

	CProxyPlayer*	GetByCharID(uint64_t u64RoleID);

	CProxyPlayer*	CreateProxyPlayer(uint64_t u64RoleID);

	bool			RemoveByCharID(uint64_t u64RoleID);

	//bool			GetPlayerOnline(UINT64 u64RoleID);


private:
	CProxyPlayerMgr(void);
	~CProxyPlayerMgr(void);

	std::unordered_map<uint64_t, CProxyPlayer> mPlayers;
};

