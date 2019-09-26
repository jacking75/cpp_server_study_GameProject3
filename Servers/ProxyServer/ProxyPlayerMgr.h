#pragma once

#ifdef _MSC_BUILD

#else
#include "..\..\ServerEngine\Platform.h"
#endif

#include "..\..\ServerEngine\AVLTree.h"


class  CProxyPlayer
{
public:
	CProxyPlayer(): m_u64RoleID(0), m_dwGameSvrID(0) {}

	~CProxyPlayer() {}

public:
	UINT32	GetGameSvrID() { return m_dwGameSvrID; }

	void	SetGameSvrInfo(UINT32 dwSvrID, UINT32 dwCopyGuid);

	void	SetConnID(UINT32 dwConnID);

	UINT64	GetCharID() { return m_u64RoleID; }

	UINT32  GetConnID() { return m_dwConnID; }

	UINT32  GetCopyGuid() { return m_dwCopyGuid; }

public:
	UINT64	m_u64RoleID;

	UINT32	m_dwGameSvrID;

	UINT32  m_dwConnID;

	UINT32  m_dwCopyGuid;
};

class CProxyPlayerMgr : public ServerEngine::AVLTree<UINT64, CProxyPlayer>
{
public:
	static CProxyPlayerMgr* GetInstancePtr();

	CProxyPlayer*	GetByCharID(UINT64 u64RoleID);

	CProxyPlayer*	CreateProxyPlayer(UINT64 u64RoleID);

	bool			RemoveByCharID(UINT64 u64RoleID);

	//bool			GetPlayerOnline(UINT64 u64RoleID);


private:
	CProxyPlayerMgr(void);
	~CProxyPlayerMgr(void);
};

