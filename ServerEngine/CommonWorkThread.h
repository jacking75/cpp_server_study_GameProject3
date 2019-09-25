#pragma once

#include "CommonThreadFunc.h"
#include "CommonMsgQueue.h"
#include "IBufferHandler.h"
#include "LockFreeQueue.h"

namespace ServerEngine
{
	Th_RetName _CommonWorkThread(void* pParam);

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
		THANDLE					m_hThread;

		BOOL					m_bRun;

		IThreadHandler* m_pThreadHandler;

		UINT64					m_dwLastTick;

		ArrayLockFreeQueue<NetPacket*>   m_PacketQueue;
	};


}