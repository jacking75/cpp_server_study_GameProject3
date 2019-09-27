#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "CheckMacroDefine.h"
#include "CommonWorkThread.h"
#include "CommonEvent.h"
#include "DataBuffer.h"
#include "PacketHeader.h"

namespace ServerEngine
{
	CCommonWorkThread::CCommonWorkThread()
	{
		m_bRun = FALSE;

		m_dwLastTick = GetTickCount();
	}

	CCommonWorkThread::~CCommonWorkThread()
	{

	}

	void CCommonWorkThread::Run()
	{
		while (m_bRun)
		{
			//这就把所有的消息都处理完了
			NetPacket* pPacket = NULL;

			while (m_PacketQueue.pop(pPacket))
			{
				ASSERT(pPacket->m_dwConnID != 0);

				m_pThreadHandler->DispatchPacket(pPacket);

				pPacket->m_pDataBuffer->Release();
			}

			Sleep(1);
		}
	}

	bool CCommonWorkThread::Start()
	{
		m_bRun = TRUE;

		//TODO 수정하기
		/*m_hThread = CommonThreadFunc::CreateThreadWrapFunc(_CommonWorkThread, this);

		if (m_hThread != NULL)
		{
			return TRUE;
		}*/

		return FALSE;
	}

	bool CCommonWorkThread::Stop()
	{
		m_bRun = FALSE;

		//TODO 수정하기
		//CommonThreadFunc::WaitThreadExitWrapFunc(m_hThread);

		return TRUE;
	}


	bool CCommonWorkThread::AddMessage(NetPacket* pNetPacket)
	{
		m_PacketQueue.push(pNetPacket);
		return TRUE;
	}

	bool CCommonWorkThread::SetThreadHandler(IThreadHandler* pCommandHandler)
	{
		m_pThreadHandler = pCommandHandler;

		return TRUE;
	}

	bool CCommonWorkThread::OnThreadBegin()
	{
		if (m_pThreadHandler == NULL)
		{
			return FALSE;
		}

		m_pThreadHandler->OnThreadBegin();

		return TRUE;
	}

	bool CCommonWorkThread::OnThreadEnd()
	{
		if (m_pThreadHandler == NULL)
		{
			return FALSE;
		}

		m_pThreadHandler->OnThreadEnd();

		return TRUE;
	}


	//TODO 수정하기
	/*Th_RetName _CommonWorkThread(void* pParam)
	{
		CCommonWorkThread* pThread = (CCommonWorkThread*)pParam;

		if (!pThread->OnThreadBegin())
		{
			ASSERT_FAIELD;

			return Th_RetValue;
		}

		pThread->Run();

		pThread->OnThreadEnd();

		return Th_RetValue;
	}*/
}