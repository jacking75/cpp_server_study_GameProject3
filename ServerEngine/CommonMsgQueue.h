#pragma once

#include "Platform.h"
#include "CheckMacroDefine.h"
#include "CritSecNotify.h"

namespace CommonQueue
{
	// 스레드 세이프 하다
	// 만약 큐에 데이터가 없으면 지정 시간까지 대기한 후 false를 반환한다.
	// 큐에 데이터가 만땅이거나 빈 공간이 없을 때 일정 시간 대기를 하므로 주의해야 한다.
	template <typename ELEM_T, int dwSize=512>
	class CMessageQueue
	{
	public:
		CMessageQueue()
		{
			m_nReadPos = 0;

			m_nWritePos = 0;
		}

		~CMessageQueue()
		{
		}

		bool Pop(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(m_nReadPos == m_nWritePos)
			{
				m_CritSec.Wait();
				return false;
			}

			_Value = m_vtData[m_nReadPos];

			m_nReadPos = (m_nReadPos+1)%dwSize;
			
			m_CritSec.Unlock();
			
			return true;
		}

		bool Push(ELEM_T &_Value)
		{
			m_CritSec.Lock();
			if(((m_nWritePos + 1)%dwSize) == m_nReadPos)
			{
				m_CritSec.Unlock();
				ASSERT_FAIELD;
				return false;
			}

			m_vtData[m_nWritePos] = _Value;

			m_nWritePos = (m_nWritePos+1)%dwSize;

			m_CritSec.Unlock();

			m_CritSec.Notify();

			return true; 
		}

		ELEM_T m_vtData[dwSize];

		int m_nWritePos;
		
		int m_nReadPos;

		ServerEngine::CCritSecNotify m_CritSec;
	};


    template <typename ELEM_T, unsigned int dwSize=512>
    class CRingQueue
    {
    public:
        CRingQueue()
        {
            m_nReadIndex    = 0;
            m_nWriteIndex   = 0;
        }

        ~CRingQueue()
        {

        }

        bool Pop(ELEM_T &_Value)
        {
            if(m_nReadIndex == m_nWriteIndex)
            {
                return false;
            }

            _Value = m_vtData[m_nReadIndex];

            m_nReadIndex = (m_nReadIndex+1)%dwSize;

            return true;
        }

		bool Push(ELEM_T &_Value)
        {
            if(((m_nWriteIndex + 1)%dwSize) == m_nReadIndex)
            {
                return false;
            }

            m_vtData[m_nWriteIndex] = _Value;

            m_nWriteIndex = (m_nWriteIndex+1)%dwSize;

            return true; 
        }

        ELEM_T m_vtData[dwSize];

        int m_nWriteIndex;

        int m_nReadIndex;
    };
}


