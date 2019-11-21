﻿#pragma once

#include "CommonFunc.h"

#include <cstdint>

namespace ServerEngine
{
	class CTimerSlotBase
	{
	public:
		virtual ~CTimerSlotBase() {}
		virtual bool operator()(uint32_t pData) { return true; }
		virtual void* GetThisAddr() { return 0; }
	};



	template<typename T>
	class CTimerSlot : public CTimerSlotBase
	{
		typedef bool(T::* FuncType)(uint32_t);
	public:
		CTimerSlot(bool(T::* FuncType)(uint32_t), T* pObj)
			: m_pFuncPtr(FuncType), m_pThis(pObj)
		{

		}
		virtual ~CTimerSlot() {}

		virtual bool operator() (uint32_t pData)
		{
			if (m_pThis != NULL && m_pFuncPtr != NULL)
			{
				(m_pThis->*m_pFuncPtr)(pData);
				return true;
			}
			else
			{
				return false;
			}
		}

		virtual void* GetThisAddr()
		{
			return reinterpret_cast<void*>(m_pThis);
		}

	private:
		FuncType m_pFuncPtr;
		T* m_pThis;
	};

	struct TimeEvent
	{
	public:
		TimeEvent()
		{
			m_dwFireTime = 0;
			m_dwSec = 0;
			m_dwData = 0;
			m_pNext = NULL;
			m_pPrev = NULL;
			m_dwRepeateTimes = 0x0FFFFFFF;
			m_pTimerFuncSlot = NULL;
		}

		~TimeEvent()
		{
			Reset();
		}

		void Reset()
		{
			m_dwFireTime = 0;
			m_dwSec = 0;
			m_dwData = 0;
			m_pNext = NULL;
			m_pPrev = NULL;
			m_dwRepeateTimes = 0x0FFFFFFF;
			if (m_pTimerFuncSlot != NULL)
			{
				delete m_pTimerFuncSlot;
			}
		}

		uint64_t m_dwFireTime;  //트리거 시간
		uint32_t m_dwSec;
		uint32_t m_dwData;
		TimeEvent* m_pPrev; //이전 노드
		TimeEvent* m_pNext; //다음 노드
		uint32_t  m_dwType;   //이벤트 유형, 1 절대 시간 타이머, 2 상대 시간 타이머
		int32_t   m_dwRepeateTimes;
		CTimerSlotBase* m_pTimerFuncSlot;
	};


	class TimerManager
	{
		TimerManager();
		~TimerManager();

	public:
		static TimerManager* GetInstancePtr();

	public:

		template<typename T>
		bool AddFixTimer(uint32_t dwSec, uint32_t dwData, bool(T::* FuncPtr)(uint32_t), T* pObj)
		{
			TimeEvent* pNewEvent = NULL;
			if (m_pFree == NULL)
			{
				pNewEvent = new TimeEvent;
			}
			else
			{
				pNewEvent = m_pFree;
				m_pFree = m_pFree->m_pNext;
				m_pFree->m_pPrev = NULL;
			}

			pNewEvent->m_pNext = NULL;
			pNewEvent->m_pPrev = NULL;

			pNewEvent->m_dwData = dwData;
			pNewEvent->m_dwFireTime = CommonFunc::GetDayBeginTime() + dwSec;

			pNewEvent->m_dwSec = dwSec;
			pNewEvent->m_dwType = 1;
			pNewEvent->m_pTimerFuncSlot = new CTimerSlot<T>(FuncPtr, pObj);

			if (m_pHead == NULL)
			{
				m_pHead = pNewEvent;
			}
			else
			{
				pNewEvent->m_pNext = m_pHead;
				m_pHead->m_pPrev = pNewEvent;
				m_pHead = pNewEvent;
				m_pHead->m_pPrev = NULL;
			}
					
			return true;
		}

		template<typename T>
		bool AddDiffTimer(uint32_t dwSec, uint32_t dwData, bool(T::* FuncPtr)(uint32_t), T* pObj)
		{
			TimeEvent* pNewEvent = NULL;
			if (m_pFree == NULL)
			{
				pNewEvent = new TimeEvent;
			}
			else
			{
				pNewEvent = m_pFree;
				m_pFree = m_pFree->m_pNext;
				m_pFree->m_pPrev = NULL;
			}

			pNewEvent->m_pNext = NULL;
			pNewEvent->m_pPrev = NULL;

			pNewEvent->m_dwData = dwData;

			pNewEvent->m_dwFireTime = CommonFunc::GetDayBeginTime() + dwSec;
			pNewEvent->m_dwSec = dwSec;
			pNewEvent->m_dwType = 2;

			pNewEvent->m_pTimerFuncSlot = new CTimerSlot<T>(FuncPtr, pObj);
			if (m_pHead == NULL)
			{
				m_pHead = pNewEvent;
			}
			else
			{
				pNewEvent->m_pNext = m_pHead;
				m_pHead->m_pPrev = pNewEvent;
				m_pHead = pNewEvent;
				m_pHead->m_pPrev = NULL;
			}

			return true;
		}

		bool DelTimer(uint32_t dwSec, uint32_t dwData);

		void UpdateTimer();

		void OnTimerEvent(TimeEvent* pEvent);

		bool InitTimer();

		bool Clear();

		TimeEvent* m_pHead;

		TimeEvent* m_pFree;

		uint64_t     m_dwCurTime;

		uint64_t     m_dwInitTime;  //타이머 작동 시작(작업 시간 시작 전 타이머가 아님)
	
	};



}