#pragma once

#include "CommonMsgQueue.h"
#include "IBufferHandler.h"
#include "LockFreeQueue.h"

namespace ServerEngine
{	
	class CCommonWorkThread
	{
	public:
		CCommonWorkThread();

		~CCommonWorkThread();

		BOOL			Start();

		BOOL			Stop();

		void			Run();

		BOOL			AddMessage(NetPacket* pNetPacket);

		BOOL			SetThreadHandler(IThreadHandler* pThreadHandler);

		BOOL			OnThreadBegin();

		BOOL			OnThreadEnd();

	protected:
#ifdef _MSC_BUILD
		HANDLE					m_hThread;
#else
		pthread_t					m_hThread;
#endif
		BOOL					m_bRun;

		IThreadHandler* m_pThreadHandler;

		UINT64					m_dwLastTick;

		ArrayLockFreeQueue<NetPacket*>   m_PacketQueue;
	};


}