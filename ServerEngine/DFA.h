#pragma once

#include <map>
#include <set>
#include <string>

#ifdef _MSC_BUILD
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include "Platform.h"
#endif

namespace ServerEngine
{
	class DFANode;
	typedef char KeyType;
	typedef std::map<KeyType, DFANode*> DFANodeMap;
	typedef std::set<KeyType> DFANodeSet;

	class DFANode
	{
	public:
		DFANode(KeyType key);

		~DFANode();

		DFANode* AddNode(KeyType key);

		bool AddNode(const KeyType* key, INT32 len);

		DFANode* GetNode(KeyType key) const;

		bool HasKeyWord(const KeyType* pdata, INT32 len, bool bReturn = TRUE);

		bool LoadFile(const std::string& filename);

		bool IsFileterWord(const std::string& word);

		bool FilterKeyWords(std::string& word, const std::string& dest = "*");

	private:
		void GetKeyPos(INT32& nStart, INT32& nLen);

		void ClearChildNode();

		bool HasEnding();

		INT32 GetChildCount() const;
	protected:
		KeyType		m_key;
		INT32		m_start;
		DFANodeMap	m_childNodeMap;

		static INT32 s_length;
		static INT32 s_maxlen;
	};

}