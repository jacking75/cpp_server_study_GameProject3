#pragma once

#include <deque>

#include "IBufferHandler.h"
//#include "google/protobuf/message.h"
#include "ConfigFile.h"
#include "SpinLock.h"

namespace ServerEngine
{
	class Connection;

	class ServiceBase : public IDataHandler//, public CEventFuncManager
	{
	protected:
		ServiceBase(void);
		virtual ~ServiceBase(void);
	public:
		static ServiceBase* GetInstancePtr();

		bool            StartNetwork(UINT16 nPortNum, UINT32 nMaxConn, IPacketDispatcher* pDispather, std::string strListenIp = "");

		bool            StopNetwork();

		bool			OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection);

		bool			OnCloseConnect(CConnection* pConnection);

		bool			OnNewConnect(CConnection* pConnection);

		CConnection* ConnectTo(std::string strIpAddr, UINT16 sPort);

		template<typename T>
		bool			SendMsgStruct(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, T& Data);

		//bool			SendMsgProtoBuf(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const google::protobuf::Message& pdata);

		bool			SendMsgRawData(UINT32 dwConnID, UINT32 dwMsgID, UINT64 u64TargetID, UINT32 dwUserData, const char* pdata, UINT32 dwLen);

		bool			SendMsgBuffer(UINT32 dwConnID, IDataBuffer* pDataBuffer);

		CConnection* GetConnectionByID(UINT32 dwConnID);

		bool			Update();

	protected:
		IPacketDispatcher* m_pPacketDispatcher;

		std::deque<NetPacket>* m_pRecvDataQueue;
		std::deque<NetPacket>* m_pDispathQueue;
		CSpinLock							m_QueueLock;

		//以下用于统计
		UINT64								m_dwLastTick;
		UINT32								m_dwRecvNum;
		UINT32								m_dwSendNum;
		UINT32								m_dwFps;
	};


}