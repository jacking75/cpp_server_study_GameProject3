#pragma once

#include <string>
#include <thread>


#include "Connection.h"
#include "IBufferHandler.h"
#include "CommonMsgQueue.h"

namespace ServerEngine
{
	//Th_RetName _NetEventThread(void* pParam);
	//Th_RetName _NetListenThread(void* pParam);

	struct EventNode
	{
		UINT32		dwEvent;
		void* pPtr;
	};

	////////////////////////////////////////////////

	class CNetManager
	{
		CNetManager(void);

		virtual ~CNetManager(void);
	public:
		static CNetManager* GetInstancePtr()
		{
			static CNetManager NetManager;

			return &NetManager;
		}
	public:
		bool	Start(UINT16 nPortNum, UINT32 nMaxConn, IDataHandler* pBufferHandler);

		bool	Close();

		bool	SendMessageByConnID(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pData, UINT32 dwLen);

		bool    SendMsgBufByConnID(UINT32 dwConnID, IDataBuffer* pBuffer);
	public:
		bool	InitNetwork();

		bool	UninitNetwork();

		bool	StartListen(UINT16 nPortNum);

		bool	StopListen();

		//以下是完成端口部分
	public:
		bool	CreateCompletePort();

		bool	DestroyCompletePort();

		bool	CreateEventThread(UINT32 nNum);

		bool    CloseEventThread();

		bool	WorkThread_ProcessEvent();

		bool	WorkThread_Listen();

		bool	EventDelete(CConnection* pConnection);

		bool	PostSendOperation(CConnection* pConnection, bool bCheck = TRUE);

		CConnection* AssociateCompletePort(SOCKET hSocket, bool bConnect);

		CConnection* ConnectTo_Sync(std::string strIpAddr, UINT16 sPort);

		CConnection* ConnectTo_Async(std::string strIpAddr, UINT16 sPort);

	public:
		SOCKET				m_hListenSocket;

		HANDLE				m_hCompletePort;

		bool				m_bCloseEvent;		//是否关闭事件处理线程

		IDataHandler* m_pBufferHandler;

//#ifdef _MSC_BUILD
//		HANDLE				 m_hListenThread;
//		std::vector<HANDLE> m_vtEventThread;
//#else
//		pthread_t				 m_hListenThread;
//		std::vector<pthread_t> m_vtEventThread;
//#endif
		std::thread m_hListenThread;
		std::vector<std::thread> m_vtEventThread;


#ifndef _MSC_BUILD

		static void SignalHandler(int nValue)
		{
			return;
		}

		bool  ClearSignal()
		{
			m_NewAct.sa_handler = CNetManager::SignalHandler;

			sigemptyset(&m_NewAct.sa_mask); //清空此信号集

			m_NewAct.sa_flags = 0;

			sigaction(SIGPIPE, &m_NewAct, &m_OldAct);

			return true;
		}

		bool RestoreSignal()
		{
			sigaction(SIGPIPE, &m_OldAct, NULL); //恢复成原始状态

			return true;
		}

		struct sigaction m_NewAct, m_OldAct;

#endif

	};

}

