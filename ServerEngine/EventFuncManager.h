#pragma once

#include <map>
#include <vector>

#include "Platform.h"


namespace ServerEngine
{
	class CFunctionSlotBase
	{
	public:
		virtual ~CFunctionSlotBase() {}
		virtual bool operator()(void* pdata) { return TRUE; }
		virtual UINT32 GetParam() { return 0; }
		virtual void* GetThisAddr() { return 0; }
		virtual void EmptyThisAddr() {}
	};



	template<typename T, typename T2>
	class CFunctionSlot : public CFunctionSlotBase
	{
		typedef bool(T::* FuncType)(T2*);
	public:
		CFunctionSlot(bool(T::* FuncType)(T2*), T* pObj, UINT32 dwParam = 0)
			: m_pFuncPtr(FuncType), m_pThis(pObj), m_dwParam(dwParam)
		{

		}
		virtual ~CFunctionSlot() {}

		virtual bool operator() (void* pData)
		{
			if (m_pThis != NULL && m_pFuncPtr != NULL)
			{
				(m_pThis->*m_pFuncPtr)(reinterpret_cast<T2*>(pData));
				return true;
			}
			else
			{
				return false;
			}
		}

		virtual UINT32 getParma()
		{
			return m_dwParam;
		}

		virtual void* GetThisAddr()
		{
			return reinterpret_cast<void*>(m_pThis);
		}

		virtual void EmptyThisAddr()
		{
			m_pThis = NULL;
		}


	private:
		FuncType m_pFuncPtr;
		T* m_pThis;
		UINT32	m_dwParam;
	};

	class CEventFuncManager
	{
	public:
		~CEventFuncManager()
		{
			for (FUNC_MAP_TYPE::iterator it = m_FuncMap.begin(); it != m_FuncMap.end(); it++)
			{
				std::vector<CFunctionSlotBase* >* p = it->second;
				for (std::vector<CFunctionSlotBase* >::iterator it2 = p->begin(); it2 != p->end(); it2++)
				{
					delete* it2;
				}
				p->clear();
				delete p;
			}
			m_FuncMap.clear();
		}

		template<typename T, typename T2>
		bool RegisterMessageHandle(int nMsgID, bool(T::* FuncPtr)(T2*), T* pObj, int nParam = 0)
		{
			CFunctionSlotBase* pSlot = new CFunctionSlot<T, T2>(FuncPtr, pObj, nParam);
			if (pSlot == NULL)
			{
				return false;
			}

			std::vector<CFunctionSlotBase*>* pMsgVector = NULL;
			FUNC_MAP_TYPE::iterator it = m_FuncMap.find(nMsgID);
			if (it == m_FuncMap.end())
			{
				pMsgVector = new std::vector<CFunctionSlotBase*>;
				pMsgVector->push_back(pSlot);
				m_FuncMap.insert(std::make_pair(nMsgID, pMsgVector));
			}
			else
			{
				pMsgVector = it->second;
				pMsgVector->push_back(pSlot);
			}

			return true;
		}

		template<typename T>
		bool UnregisterMessageHandle(int nMsgID, T* pObj)
		{
			std::vector<CFunctionSlotBase* >* vec = NULL;
			FUNC_MAP_TYPE::iterator it = m_FuncMap.find(nMsgID);
			if (it == m_FuncMap.end())
			{
				return true;
			}
			else
			{
				vec = it->second;
			}

			for (std::vector<CFunctionSlotBase* >::iterator itor = vec->begin(); itor != vec->end(); ++itor)
			{
				if ((*itor)->GetThisAddr() == reinterpret_cast<void*>(pObj))
				{
					//反注册并不真实删除，只是将this指针置空
					(*itor)->EmptyThisAddr();
				}

			}
			return true;
		}
		template<typename T>
		bool FireMessage(int nMsgID, T* pData)
		{
			FUNC_MAP_TYPE::iterator typeIt = m_FuncMap.find(nMsgID);
			if (typeIt == m_FuncMap.end())
			{
				return false;
			}

			for (std::vector<CFunctionSlotBase* >::iterator itor = typeIt->second->begin(); itor != typeIt->second->end(); itor++)
			{
				if ((*itor)->GetThisAddr() != 0)
				{
					(**itor)((void*)pData);
				}
			}
			return true;
		}

	protected:
		std::map<int, std::vector<CFunctionSlotBase*>* > m_FuncMap;
		typedef std::map< int, std::vector<CFunctionSlotBase* >* > FUNC_MAP_TYPE;
	};

}
