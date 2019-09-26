#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "..\..\ServerEngine\Log.h"
#include "ProxyPlayerMgr.h"


void CProxyPlayer::SetGameSvrInfo( UINT32 dwSvrID, UINT32 dwCopyGuid )
{
	m_dwGameSvrID = dwSvrID;
	m_dwCopyGuid = dwCopyGuid;
}


void CProxyPlayer::SetConnID(UINT32 dwConnID)
{
	m_dwConnID = dwConnID;
}

CProxyPlayerMgr::CProxyPlayerMgr(void)
{
}

CProxyPlayerMgr::~CProxyPlayerMgr(void)
{
}

CProxyPlayerMgr* CProxyPlayerMgr::GetInstancePtr()
{
	static CProxyPlayerMgr _StaicPlayerMgr;

	return &_StaicPlayerMgr;
}

CProxyPlayer* CProxyPlayerMgr::GetByCharID(UINT64 u64RoleID)
{
	return GetByKey(u64RoleID);
}

CProxyPlayer* CProxyPlayerMgr::CreateProxyPlayer(UINT64 u64RoleID)
{
	CProxyPlayer* pClientObj = InsertAlloc(u64RoleID);
	ERROR_RETURN_NULL(pClientObj != NULL);

	pClientObj->m_u64RoleID = u64RoleID;

	return pClientObj;
}

bool CProxyPlayerMgr::RemoveByCharID(UINT64 u64RoleID)
{
	return Delete(u64RoleID);
}


