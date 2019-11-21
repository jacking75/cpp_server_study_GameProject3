#pragma once

#include "CommonMsgQueue.h"
#include "IBufferHandler.h"
#include "LockFreeQueue.h"

#include <thread>

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
		void ThreadFunc();


		bool					m_bRun;

		std::thread m_hThread;

		IThreadHandler* m_pThreadHandler;

		UINT64					m_dwLastTick;

		ArrayLockFreeQueue<NetPacket*>   m_PacketQueue;
	};


}