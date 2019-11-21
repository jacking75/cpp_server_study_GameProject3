#include "ServiceBase.h"
#include "CheckMacroDefine.h"
#include "Log.h"
#include "NetManager.h"
#include "CommonSocket.h"
#include "CommonEvent.h"
#include "DataBuffer.h"
#include "Connection.h"
#include "TimerManager.h"
#include "PacketHeader.h"

namespace ServerEngine
{
#define NEW_CONNECTION 1
#define CLOSE_CONNECTION 2

	ServiceBase::ServiceBase(void)
	{
		m_pPacketDispatcher = NULL;
		m_pRecvDataQueue = new std::deque<NetPacket>();
		m_pDispathQueue = new std::deque<NetPacket>();
	}

	ServiceBase::~ServiceBase(void)
	{
		delete m_pRecvDataQueue;
		delete m_pDispathQueue;
	}

	ServiceBase* ServiceBase::GetInstancePtr()
	{
		static ServiceBase _ServiceBase;

		return &_ServiceBase;
	}


	bool ServiceBase::OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection)
	{
		PacketHeader* pHeader = (PacketHeader*)pDataBuffer->GetBuffer();

		m_QueueLock.Lock();
		m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), pDataBuffer, pHeader->dwMsgID));
		m_QueueLock.Unlock();
		return true;
	}

	bool ServiceBase::StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp)
	{
		if (pDispather == NULL)
		{
			ASSERT_FAIELD;
			return false;
		}

		if ((nPortNum <= 0) || (nMaxConn <= 0))
		{
			ASSERT_FAIELD;
			return false;
		}

		m_pPacketDispatcher = pDispather;

		if (!CNetManager::GetInstancePtr()->Start(nPortNum, nMaxConn, this))
		{
			CLog::GetInstancePtr()->LogError("네트워크 계층을 시작하지 못했습니다!");
			return false;
		}

		m_dwLastTick = 0;
		m_dwRecvNum = 0;
		m_dwFps = 0;
		m_dwSendNum = 0;
		return true;
	}

	bool ServiceBase::StopNetwork()
	{
		CLog::GetInstancePtr()->LogError("==========서버가 종료되기 시작합니다=======================");

		CNetManager::GetInstancePtr()->Close();

		CLog::GetInstancePtr()->CloseLog();

		return true;
	}

	template<typename T>
	bool ServiceBase::SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data)
	{
		if (dwConnID <= 0)
		{
			return false;
		}

		m_dwSendNum++;

		return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, &Data, sizeof(T));
	}

	//bool ServiceBase::SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata)
	//{
	//	if (dwConnID <= 0)
	//	{
	//		return false;
	//	}
	//
	//	char szBuff[102400] = {0};
	//
	//	ERROR_RETURN_FALSE(pdata.ByteSize() < 102400);
	//
	//	pdata.SerializePartialToArray(szBuff, pdata.GetCachedSize());
	//	m_dwSendNum++;
	//	return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, szBuff, pdata.GetCachedSize());
	//}

	bool ServiceBase::SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen)
	{
		if (dwConnID <= 0)
		{
			return false;
		}

		m_dwSendNum++;
		return CNetManager::GetInstancePtr()->SendMessageByConnID(dwConnID, dwMsgID, u64TargetID, dwUserData, pdata, dwLen);
	}

	bool ServiceBase::SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer)
	{
		if (dwConnID <= 0)
		{
			return false;
		}

		m_dwSendNum++;
		return CNetManager::GetInstancePtr()->SendMsgBufByConnID(dwConnID, pDataBuffer);
	}

	CConnection* ServiceBase::ConnectTo(std::string strIpAddr, UINT16 sPort)
	{
		if (strIpAddr.empty() || sPort <= 0)
		{
			ASSERT_FAIELD;
			return NULL;
		}

		return CNetManager::GetInstancePtr()->ConnectTo_Async(strIpAddr, sPort);
	}

	bool ServiceBase::OnCloseConnect(CConnection* pConnection)
	{
		ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);

		m_QueueLock.Lock();
		m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), (IDataBuffer*)pConnection, CLOSE_CONNECTION));
		m_QueueLock.Unlock();
		return true;
	}

	bool ServiceBase::OnNewConnect(CConnection* pConnection)
	{
		ERROR_RETURN_FALSE(pConnection->GetConnectionID() != 0);
		m_QueueLock.Lock();
		m_pRecvDataQueue->emplace_back(NetPacket(pConnection->GetConnectionID(), (IDataBuffer*)pConnection, NEW_CONNECTION));
		m_QueueLock.Unlock();
		return true;
	}


	CConnection* ServiceBase::GetConnectionByID(UINT32 dwConnID)
	{
		return CConnectionMgr::GetInstancePtr()->GetConnectionByConnID(dwConnID);
	}

	bool ServiceBase::Update()
	{
		if (m_dwLastTick == 0)
		{
			m_dwLastTick = GetTickCountRefFunc();
		}

		CConnectionMgr::GetInstancePtr()->CheckConntionAvalible();

		m_QueueLock.Lock();
		std::swap(m_pRecvDataQueue, m_pDispathQueue);
		m_QueueLock.Unlock();

		if (m_pDispathQueue->size() > 0)
		{
			for (std::deque<NetPacket>::iterator itor = m_pDispathQueue->begin(); itor != m_pDispathQueue->end(); itor++)
			{
				NetPacket& item = *itor;
				if (item.m_dwMsgID == NEW_CONNECTION)
				{
					m_pPacketDispatcher->OnNewConnect((CConnection*)item.m_pDataBuffer);
				}
				else if (item.m_dwMsgID == CLOSE_CONNECTION)
				{
					m_pPacketDispatcher->OnCloseConnect((CConnection*)item.m_pDataBuffer);
					//发送通知
					CConnectionMgr::GetInstancePtr()->DeleteConnection((CConnection*)item.m_pDataBuffer);
				}
				else
				{
					m_pPacketDispatcher->DispatchPacket(&item);

					item.m_pDataBuffer->Release();

					m_dwRecvNum += 1;
				}
			}

			m_pDispathQueue->clear();
		}

		m_dwFps += 1;

		if ((GetTickCountRefFunc() - m_dwLastTick) > 1000)
		{
			m_pPacketDispatcher->OnSecondTimer();
						
			m_dwFps = 0;
			m_dwRecvNum = 0;
			m_dwSendNum = 0;
			m_dwLastTick = GetTickCountRefFunc();
		}

		TimerManager::GetInstancePtr()->UpdateTimer();

		CLog::GetInstancePtr()->Flush();

		return true;
	}
}