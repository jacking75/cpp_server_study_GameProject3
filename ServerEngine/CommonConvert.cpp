#include <algorithm>
#include <string>
#include <vector>

#include "CommonConvert.h"


namespace ServerEngine
{
	INT32 ServerEngine::StringToInt(char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return atoi(pStr);
	}

	INT64 ServerEngine::StringToInt64(char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return atol(pStr);
	}

	INT64 ServerEngine::StringToInt64(const char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}


#ifdef WIN32
		return _atoi64(pStr);
#else
		return atoll(pStr);
#endif
	}

	INT32 ServerEngine::StringToInt(const char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return atoi(pStr);
	}

	FLOAT  ServerEngine::StringToFloat(char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return (FLOAT)atof(pStr);
	}

	DOUBLE ServerEngine::StringToDouble(char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return strtod(pStr, NULL);
	}

	std::string ServerEngine::DoubleToString(DOUBLE dValue)
	{
		CHAR szValue[64] = { 0 };

		snprintf(szValue, 64, "%f", dValue);

		return std::string(szValue);
	}

	FLOAT  ServerEngine::StringToFloat(const char* pStr)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		return (FLOAT)atof(pStr);
	}

	std::string ServerEngine::IntToString(INT32 nValue)
	{
		CHAR szValue[64] = { 0 };

		snprintf(szValue, 64, "%d", nValue);

		return std::string(szValue);
	}

	std::string ServerEngine::IntToString(INT64 uValue)
	{
		CHAR szValue[64] = { 0 };

		snprintf(szValue, 64, "%lld", uValue);

		return std::string(szValue);
	}

	std::string ServerEngine::IntToString(UINT32 nValue)
	{
		CHAR szValue[64] = { 0 };

		snprintf(szValue, 64, "%d", nValue);

		return std::string(szValue);
	}

	std::string ServerEngine::IntToString(UINT64 uValue)
	{
		CHAR szValue[64] = { 0 };

		snprintf(szValue, 64, "%lld", uValue);

		return std::string(szValue);
	}

	bool ServerEngine::StringToPos(char* pStr, FLOAT& x, FLOAT& y, FLOAT& z)
	{
		if (pStr == NULL)
		{
			return false;
		}

		char szTempBuf[256] = { 0 };
		strncpy_s(szTempBuf, 256, pStr, strlen(pStr));

		char* pPos = strchr(szTempBuf, ',');
		if (pPos == NULL)
		{
			return false;
		}
		*pPos = 0;
		x = ServerEngine::StringToFloat(szTempBuf + 1);

		char* pOldPos = pPos + 1;
		pPos = strchr(pPos + 1, ',');
		if (pPos == NULL)
		{
			return false;
		}
		*pPos = 0;
		y = ServerEngine::StringToFloat(pOldPos);

		pOldPos = pPos + 1;
		pPos = strchr(pPos + 1, ')');
		if (pPos == NULL)
		{
			return false;
		}
		*pPos = 0;
		z = ServerEngine::StringToFloat(pOldPos);

		return true;
	}

	std::string ServerEngine::FloatToString(FLOAT fValue, INT32 nPrecision, bool bRound)
	{
		CHAR szValue[64] = { 0 };

		if ((bRound) && (nPrecision > 0) && (nPrecision < 6))
		{
			FLOAT fRoundValue = 5;

			for (int i = 0; i < nPrecision + 1; i++)
			{
				fRoundValue *= 0.1f;
			}

			fValue += fRoundValue;
		}

		snprintf(szValue, 64, "%f", fValue);

		CHAR* pChar = strchr(szValue, '.');
		if (pChar == NULL)
		{
			return std::string(szValue);
		}

		*(pChar + nPrecision + 1) = '\0';

		return std::string(szValue);
	}

	bool ServerEngine::SpliteString(std::string strSrc, std::string strDelim, std::vector<std::string>& vtStr)
	{
		vtStr.clear();
		if (strDelim.empty())
		{
			vtStr.push_back(strSrc);
			return true;
		}

		std::string::iterator subStart, subEnd;
		subStart = strSrc.begin();
		while (true)
		{
			subEnd = std::search(subStart, strSrc.end(), strDelim.begin(), strDelim.end());
			std::string temp(subStart, subEnd);
			if (!temp.empty())
			{
				vtStr.push_back(temp);
			}
			if (subEnd == strSrc.end())
			{
				break;
			}
			subStart = subEnd + strDelim.size();
		}

		return true;
	}

	bool ServerEngine::ReplaceString(std::string& str, const std::string& pattern, const std::string& newpat)
	{
		const size_t nsize = newpat.size();
		const size_t psize = pattern.size();

		for (size_t pos = str.find(pattern, 0); pos != std::string::npos; pos = str.find(pattern, pos + nsize))
		{
			str.replace(pos, psize, newpat);
		}

		return true;
	}

	bool ServerEngine::StringToVector(const char* pStrValue, INT32 IntVector[], INT32 nSize, char cDelim)
	{
		if (pStrValue == NULL)
		{
			return false;
		}

		char szBuf[1024] = { 0 };
		strncpy_s(szBuf, 1024, pStrValue, 1024);

		char* pBeginPos = szBuf;
		char* pEndPos = strchr(pBeginPos, cDelim);

		if (pBeginPos == pEndPos)
		{
			pBeginPos += 1;
			pEndPos = strchr(pBeginPos, cDelim);
		}

		INT32 nIndex = 0;
		while (pEndPos != NULL)
		{
			//*pEndPos = 0;
			IntVector[nIndex++] = StringToInt(pBeginPos);
			if (nIndex >= nSize)
			{
				return true;
			}

			pBeginPos = pEndPos + 1;
			pEndPos = strchr(pBeginPos, cDelim);
		}

		if (*pBeginPos != 0 && nIndex < nSize)
		{
			IntVector[nIndex++] = StringToInt(pBeginPos);
		}

		return true;
	}

	bool ServerEngine::SpliteString(std::string strSrc, char cDelim, std::vector<std::string>& vtStr)
	{
		vtStr.clear();

		std::string::size_type posStart, posEnd;
		posEnd = strSrc.find(cDelim);
		posStart = 0;
		while (std::string::npos != posEnd)
		{
			vtStr.emplace_back(strSrc.substr(posStart, posEnd - posStart));

			posStart = posEnd + 1;
			posEnd = strSrc.find(cDelim, posStart);
		}

		if (posStart != strSrc.length())
		{
			vtStr.emplace_back(strSrc.substr(posStart));
		}

		return true;
	}

	std::wstring ServerEngine::Utf8_To_Unicode(std::string strSrc)
	{
		wchar_t wBuff[1024] = { 0 };
#ifdef WIN32
		MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, wBuff, 1024);
		std::wstring strRet = wBuff;
		return strRet;
#else
#endif
	}

	std::string ServerEngine::Unicode_To_Uft8(std::wstring wstrValue)
	{
		CHAR sBuff[1024] = { 0 };
#ifdef WIN32
		WideCharToMultiByte(CP_UTF8, 0, wstrValue.c_str(), -1, sBuff, 1024, NULL, NULL);
		std::string strRet = sBuff;
		return strRet;
#else
#endif
	}

	std::wstring ServerEngine::Ansi_To_Unicode(std::string strSrc)
	{
		wchar_t wBuff[1024] = { 0 };
		CHAR sBuff[1024] = { 0 };
#ifdef WIN32
		MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), -1, wBuff, 1024);
		std::wstring strRet = wBuff;
		return strRet;
#else
#endif
	}

	std::string ServerEngine::Unicode_To_Ansi(std::wstring strValue)
	{
		CHAR sBuff[1024] = { 0 };
#ifdef WIN32
		WideCharToMultiByte(CP_ACP, 0, strValue.c_str(), -1, sBuff, 1024, NULL, NULL);
		return std::string(sBuff);
#else

#endif
	}

	std::string ServerEngine::Utf8_To_Ansi(std::string strSrc)
	{
		wchar_t wBuff[1024] = { 0 };
		CHAR sBuff[1024] = { 0 };
#ifdef WIN32
		MultiByteToWideChar(CP_UTF8, 0, strSrc.c_str(), -1, wBuff, 1024);
		WideCharToMultiByte(CP_ACP, 0, wBuff, -1, sBuff, 1024, NULL, NULL);
		std::string strRet = sBuff;
		return strRet;
#else
#endif
	}

	std::string ServerEngine::Ansi_To_Uft8(std::string strSrc)
	{
		wchar_t wBuff[1024] = { 0 };
		CHAR sBuff[1024] = { 0 };
#ifdef WIN32
		MultiByteToWideChar(CP_ACP, 0, strSrc.c_str(), -1, wBuff, 1024);
		WideCharToMultiByte(CP_UTF8, 0, wBuff, -1, sBuff, 1024, NULL, NULL);
		std::string strRet = sBuff;
		return strRet;
#else
#endif
	}

	bool ServerEngine::IsTextUTF8(const char* str, UINT32 length)
	{
		UINT32 i;
		UINT32 nBytes = 0;//UFT8可用1-6个字节编码,ASCII用一个字节
		UINT8 chr;
		bool bAllAscii = true; //如果全部都是ASCII, 说明不是UTF-8
		for (i = 0; i < length; i++)
		{
			chr = *(str + i);
			if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			{
				bAllAscii = false;
			}
			if (nBytes == 0) //如果不是ASCII码,应该是多字节符,计算字节数
			{
				if (chr >= 0x80)
				{
					if (chr >= 0xFC && chr <= 0xFD)
					{
						nBytes = 6;
					}
					else if (chr >= 0xF8)
					{
						nBytes = 5;
					}
					else if (chr >= 0xF0)
					{
						nBytes = 4;
					}
					else if (chr >= 0xE0)
					{
						nBytes = 3;
					}
					else if (chr >= 0xC0)
					{
						nBytes = 2;
					}
					else
					{
						return false;
					}
					nBytes--;
				}
			}
			else //多字节符的非首字节,应为 10xxxxxx
			{
				if ((chr & 0xC0) != 0x80)
				{
					return false;
				}
				nBytes--;
			}
		}
		if (nBytes > 0) //违返规则
		{
			return false;
		}
		if (bAllAscii) //如果全部都是ASCII, 说明不是UTF-8
		{
			return false;
		}
		return true;
	}

	UINT32 ServerEngine::VersionToInt(std::string& strVersion)
	{
		INT32 nValue[3] = { 0 };
		StringToVector(strVersion.c_str(), nValue, 3, '.');
		return nValue[0] * 1000000 + nValue[1] * 1000 + nValue[2];
	}

	INT32 ServerEngine::CountSymbol(char* pStr, char cSymbol)
	{
		if (pStr == NULL)
		{
			return 0;
		}

		INT32 nCount = 0;

		char* pTemp = pStr;
		while (*pTemp != '\0')
		{
			if (*pTemp == cSymbol)
			{
				nCount += 1;
			}

			pTemp += 1;
		}

		return nCount;
	}

	bool ServerEngine::StringTrim(std::string& strValue)
	{
		if (!strValue.empty())
		{
			strValue.erase(0, strValue.find_first_not_of((" \n\r\t")));
			strValue.erase(strValue.find_last_not_of((" \n\r\t")) + 1);
		}
		return true;
	}
}