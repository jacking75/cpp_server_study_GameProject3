#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#include "Platform.h"

namespace ServerEngine
{
	class CConnection;
	class CBufferHelper;
	struct NetPacket;

	class IDataBuffer
	{
	public:
		virtual INT32	GetRef() = 0;

		virtual bool	AddRef() = 0;

		virtual bool	Release() = 0;

		virtual CHAR* GetData() = 0;

		virtual UINT32	GetTotalLenth() = 0;

		virtual void	SetTotalLenth(UINT32 nPos) = 0;

		virtual UINT32  GetBodyLenth() = 0;

		virtual CHAR* GetBuffer() = 0;

		virtual UINT32	GetBufferSize() = 0;

		virtual UINT32  CopyFrom(IDataBuffer* pSrcBuffer) = 0;

		virtual UINT32  CopyTo(CHAR* pDestBuf, UINT32 dwDestLen) = 0;
	};

	struct IDataHandler
	{
		virtual bool OnDataHandle(IDataBuffer* pDataBuffer, CConnection* pConnection) = 0;
		virtual bool OnCloseConnect(CConnection* pConnection) = 0;
		virtual bool OnNewConnect(CConnection* pConnection) = 0;
	};

	struct IPacketDispatcher
	{
		virtual bool DispatchPacket(NetPacket* pNetPacket) = 0;
		virtual bool OnSecondTimer() = 0;
		virtual bool OnCloseConnect(CConnection* pConnection) = 0;
		virtual bool OnNewConnect(CConnection* pConnection) = 0;
	};

	struct  IThreadHandler
	{
		virtual bool DispatchPacket(NetPacket* pNetPacket) = 0;
		virtual bool OnThreadBegin() = 0;
		virtual bool OnThreadEnd() = 0;
	};


	struct NetPacket
	{
		NetPacket(UINT32 dwConnID = 0, IDataBuffer* pBuffer = NULL, UINT32 dwMsgID = 0)
		{
			m_dwConnID = dwConnID;

			m_pDataBuffer = pBuffer;

			m_dwMsgID = dwMsgID;
		}

		UINT32       m_dwMsgID;
		UINT32       m_dwConnID;
		IDataBuffer* m_pDataBuffer;
	};




}
