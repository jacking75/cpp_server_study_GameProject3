#pragma once

#include <atomic>

namespace ServerEngine
{
	class CSpinLock
	{
		std::atomic_flag m_flag = ATOMIC_FLAG_INIT;

	public:
		CSpinLock()
		{

		};

		~CSpinLock()
		{
		};

		void Lock()
		{
			for (unsigned k = 0; TryLock(); ++k)
			{
				if (k % 1024 == 0)
				{
					//TODO 수정하기
					//CommonFunc::Sleep(1);
				}
			}

			return;
		}

		bool TryLockTimes(unsigned nTimes)
		{
			for (unsigned k = 0; TryLock(); ++k)
			{
				if (k >= nTimes)
				{
					return false;
				}
			}

			return true;
		}

		bool TryLock()
		{
			bool bRet = m_flag.test_and_set(/*std::memory_order_acquire*/);

			return bRet;
		}

		void Unlock()
		{
			m_flag.clear(/*std::memory_order_release*/);

			return;
		}

	};


}