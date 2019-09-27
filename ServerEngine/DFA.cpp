#define _CRT_SECURE_NO_WARNINGS  //TODO �� ��ũ�� �ڿ� �����ؾ� �Ѵ�

#include <algorithm>

#include "Platform.h"
#include "DFA.h"

namespace ServerEngine
{
	INT32 DFANode::s_length = 0;
	INT32 DFANode::s_maxlen = 0;

	//--------------------------------------------------------------------------------------------------------
	DFANode::DFANode(KeyType key) : m_key(key), m_start(0)
	{

	}
	//--------------------------------------------------------------------------------------------------------
	DFANode::~DFANode()
	{
		ClearChildNode();
	}
	//--------------------------------------------------------------------------------------------------------
	bool DFANode::AddNode(const KeyType* key, INT32 len)
	{
		if (key == NULL || len < 0)
		{
			return false;
		}

		int i = 0;
		DFANode* ptemNode = this;
		while (i < len)
		{
			ptemNode = ptemNode->AddNode(key[i]);
			++i;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------
	DFANode* DFANode::AddNode(KeyType key)
	{
		DFANodeMap::iterator it = m_childNodeMap.find(key);

		if (it == m_childNodeMap.end())
		{
			DFANode* temnode = new DFANode(key);
			m_childNodeMap.insert(std::make_pair(key, temnode));
			return temnode;
		}
		else
		{
			return it->second;
		}
	}
	//--------------------------------------------------------------------------------------------------------
	DFANode* DFANode::GetNode(KeyType key) const
	{
		DFANodeMap::const_iterator  it = m_childNodeMap.find(key);
		if (it == m_childNodeMap.end())
		{
			return NULL;
		}
		else
		{
			return it->second;
		}

	}
	//--------------------------------------------------------------------------------------------------------
	bool DFANode::HasKeyWord(const KeyType* pdata, int len, bool bReturn/* = true*/)
	{
		if (bReturn)
		{
			s_length = 0;
			s_maxlen = 0;
		}

		INT32 currentPos = 0;
		while (currentPos < len)
		{
			KeyType p = pdata[currentPos];
			DFANode* pnode = GetNode(p);
			++s_length;
			if (pnode == NULL)
			{
				if (bReturn == false)
				{
					return false;
				}

				++currentPos;
				s_length = 0;
				continue;
			}
			else if (pnode->HasEnding())
			{
				s_maxlen = s_length;

				if (bReturn)
				{
					m_start = currentPos;
				}

				if (pnode->GetChildCount() <= 1)
				{
					return true;
				}
			}

			int startpos = currentPos + 1;
			if (startpos >= len)
			{
				return s_maxlen != 0;
			}

			bool b = pnode->HasKeyWord(pdata + startpos, len - startpos, false);

			if (b == true)
			{
				if (bReturn)
				{
					m_start = currentPos;
				}

				return true;
			}
			else
			{
				if (bReturn == false)
				{
					return false;
				}
				else if (s_maxlen != 0)
				{
					s_length = s_maxlen;
					return true;
				}

				++currentPos;
				s_length = 0;
				continue;
			}
		}

		return false;
	}
	//--------------------------------------------------------------------------------------------------------
	bool DFANode::LoadFile(const std::string& filename)
	{
		ClearChildNode();

		FILE* pFile = fopen(filename.c_str(), "r+");

		if (pFile == NULL)
		{
			return false;
		}

		char line[1024] = { 0 };
		do
		{
			fgets(line, 1024, pFile);

			int nLen = (int)strlen(line);
			if (line[nLen - 1] == '\n')
			{
				line[nLen - 1] = 0;
			}

			if (line[nLen - 1] == ' ')
			{
				line[nLen - 1] = 0;
			}

			if (strcmp(line, "*") == 0)
			{
				continue;
			}

			strcat(line, "#");

			std::string tmpline = line;

			std::transform(tmpline.begin(), tmpline.end(), tmpline.begin(), toupper);

			AddNode(tmpline.c_str(), (int)tmpline.size());

		} while (!feof(pFile));

		fclose(pFile);

		return true;
	}
	//--------------------------------------------------------------------------------------------------------
	bool DFANode::IsFileterWord(const std::string& word)
	{
		std::string wpData(word);

		std::transform(wpData.begin(), wpData.end(), wpData.begin(), toupper);

		return HasKeyWord(wpData.c_str(), (int)wpData.size());
	}

	//--------------------------------------------------------------------------------------------------------
	bool DFANode::FilterKeyWords(std::string& word, const std::string& dest /*= "***"*/)
	{
		if (word.empty())
		{
			return false;
		}

		std::string wpData = word;

		std::transform(wpData.begin(), wpData.end(), wpData.begin(), toupper);


		int start = 0, strlen = 0;
		int lastPos = -1;
		bool keyflag = false;

		while (HasKeyWord(wpData.c_str(), (int)wpData.size()))
		{
			GetKeyPos(start, strlen);
			if (lastPos == start)
			{
				wpData.replace(wpData.begin() + start, wpData.begin() + start + strlen, "");
				lastPos = start;
			}
			else
			{
				wpData.replace(wpData.begin() + start, wpData.begin() + start + strlen, dest.c_str());
				lastPos = start + (int)dest.size();
			}

			keyflag = true;
		}

		if (keyflag)
		{
			word = wpData;
		}

		return true;
	}
	//--------------------------------------------------------------------------------------------------------
	void DFANode::GetKeyPos(INT32& nStart, INT32& nLen)
	{
		nStart = m_start;
		nLen = s_length;
		m_start = 0;
		s_length = 0;
	}
	//--------------------------------------------------------------------------------------------------------
	void DFANode::ClearChildNode()
	{
		DFANodeMap::iterator  itend = m_childNodeMap.end();
		for (DFANodeMap::iterator it = m_childNodeMap.begin(); it != itend; ++it)
		{
			delete (it->second);
		}

		m_childNodeMap.clear();
	}
	//--------------------------------------------------------------------------------------------------------
	bool DFANode::HasEnding()
	{
		return m_childNodeMap.empty() ? true : m_childNodeMap.find('#') != m_childNodeMap.end();
	}

	INT32 DFANode::GetChildCount() const
	{
		return (INT32)m_childNodeMap.size();
	}

}