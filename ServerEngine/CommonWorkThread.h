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

		bool			Start();

		bool			Stop();

		void			Run();

		bool			AddMessage(NetPacket* pNetPacket);

		bool			SetThreadHandler(IThreadHandler* pThreadHandler);

		bool			OnThreadBegin();

		bool			OnThreadEnd();

	protected:
#ifdef _MSC_BUILD
		HANDLE					m_hThread;
#else
		pthread_t					m_hThread;
#endif
		bool					m_bRun;

		IThreadHandler* m_pThreadHandler;

		UINT64					m_dwLastTick;

		ArrayLockFreeQueue<NetPacket*>   m_PacketQueue;
	};


}