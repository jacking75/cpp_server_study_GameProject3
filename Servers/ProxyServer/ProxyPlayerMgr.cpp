#include "ProxyPlayerMgr.h"

#include "..\..\ServerEngine\Log.h"



void CProxyPlayer::SetGameSvrInfo(uint32_t dwSvrID, uint32_t dwCopyGuid )
{
	m_dwGameSvrID = dwSvrID;
	m_dwCopyGuid = dwCopyGuid;
}


void CProxyPlayer::SetConnID(uint32_t dwConnID)
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

CProxyPlayer* CProxyPlayerMgr::GetByCharID(uint64_t u64RoleID)
{
	auto iter = mPlayers.find(u64RoleID);
	if (iter == mPlayers.end())
	{
		return nullptr;
	}

	return (CProxyPlayer*)&iter->second;
}

CProxyPlayer* CProxyPlayerMgr::CreateProxyPlayer(uint64_t u64RoleID)
{
	CProxyPlayer pClientObj;
	pClientObj.m_u64RoleID = u64RoleID;
	
	auto [iter, inserted] = mPlayers.insert({ u64RoleID, pClientObj });
	return (CProxyPlayer*)&iter->second;
}

bool CProxyPlayerMgr::RemoveByCharID(uint64_t u64RoleID)
{
	return mPlayers.erase(u64RoleID) > 0 ? true : false;
	//return Delete(u64RoleID);
}


