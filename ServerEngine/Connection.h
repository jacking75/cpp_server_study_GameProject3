#pragma once

#include <vector>

//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
#include <winsock2.h>

#include "Platform.h"

#include "IBufferHandler.h"
#include "CritSec.h"
#include "LockFreeQueue.h"

namespace ServerEngine
{
#define  NET_MSG_RECV				1
#define  NET_MSG_SEND				2
#define  NET_MSG_CONNECT			3
#define  NET_MSG_POST				4

#define RECV_BUF_SIZE               8192
#define MAX_BUFF_SIZE				32768

#define E_SEND_SUCCESS				1
#define E_SEND_UNDONE				2
#define E_SEND_ERROR				3

	struct NetIoOperatorData
	{
#ifdef _MSC_BUILD
		OVERLAPPED		Overlap;
#endif
		UINT32			dwCmdType;
		UINT32			dwConnID;

		IDataBuffer* pDataBuffer;

		void			Reset();
	};

	class CConnection
	{
	public:
		CConnection();
		virtual ~CConnection();
	public:
		bool	HandleRecvEvent(UINT32 dwBytes);

		UINT32  GetConnectionID();

		UINT64  GetConnectionData();

		void    SetConnectionID(UINT32 dwConnID);

		void	SetConnectionData(UINT64 dwData);

		bool	Close();

		bool	SetSocket(SOCKET hSocket);

		SOCKET  GetSocket();

		bool	SetDataHandler(IDataHandler* pHandler);

		bool	ExtractBuffer();

		bool	DoReceive();

		bool	IsConnectionOK();

		bool	SetConnectionOK(bool bOk);

		bool    Reset();

		bool    SendBuffer(IDataBuffer* pBuff);

		bool    DoSend();

		bool	CheckHeader(CHAR* m_pPacket);

	public:
		SOCKET						m_hSocket;

		bool						m_bConnected;

		NetIoOperatorData			m_IoOverlapRecv;

		NetIoOperatorData			m_IoOverlapSend;

		NetIoOperatorData			m_IoOverLapPost;

		UINT32                      m_dwConnID;
		UINT64                      m_u64ConnData;

		IDataHandler* m_pDataHandler;

		UINT32						m_dwIpAddr;

		UINT32						m_dwDataLen;
		CHAR						m_pRecvBuf[RECV_BUF_SIZE];
		CHAR* m_pBufPos;

		IDataBuffer* m_pCurRecvBuffer;
		UINT32						m_pCurBufferSize;
		UINT32						m_nCheckNo;

		CConnection* m_pNext;

		UINT64						m_LastRecvTick;

		ArrayLockFreeQueue < IDataBuffer* > m_SendBuffList;



		bool				        m_IsSending;

		//LINUX下专用， 用于发了一半的包
		IDataBuffer* m_pSendingBuffer;
		UINT32						m_nSendingPos;
	};


	class CConnectionMgr
	{
	private:
		CConnectionMgr();

		~CConnectionMgr();

	public:
		static CConnectionMgr* GetInstancePtr();

	public:
		bool            InitConnectionList(UINT32 nMaxCons);

		CConnection* CreateConnection();

		bool		    DeleteConnection(CConnection* pConnection);

		CConnection* GetConnectionByConnID(UINT32 dwConnID);

		///////////////////////////////////////////
		bool		    CloseAllConnection();

		bool		    DestroyAllConnection();

		bool			CheckConntionAvalible();

	public:

		CConnection* m_pFreeConnRoot;
		CConnection* m_pFreeConnTail;
		std::vector<CConnection*>	m_vtConnList;            //连接列表
		CCritSec					m_CritSecConnList;
	};

}