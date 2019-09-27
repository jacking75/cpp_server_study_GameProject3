﻿#pragma once

#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Platform.h"

namespace ServerEngine
{
	INT32 StringToInt(char* pStr);

	INT32 StringToInt(const char* pStr);

	INT64 StringToInt64(char* pStr);

	INT64 StringToInt64(const char* pStr);

	FLOAT StringToFloat(char* pStr);

	FLOAT StringToFloat(const char* pStr);

	DOUBLE StringToDouble(char* pStr);

	std::string DoubleToString(DOUBLE dValue);

	std::string IntToString(UINT32 nValue);

	std::string IntToString(INT32 nValue);

	std::string IntToString(UINT64 nValue);

	std::string IntToString(INT64 nValue);

	bool  StringToPos(char* pStr, FLOAT& x, FLOAT& y, FLOAT& z);

	//浮点到字符串， nPrecision 保留的最大小数的位数， bRound 是否四舍五入
	std::string FloatToString(FLOAT fValue, INT32 nPrecision = -1, bool bRound = FALSE);

	std::wstring Utf8_To_Unicode(std::string strValue);

	std::string  Unicode_To_Uft8(std::wstring wstrValue);

	std::wstring Ansi_To_Unicode(std::string strValue);

	std::string Unicode_To_Ansi(std::wstring strValue);

	std::string Utf8_To_Ansi(std::string strValue);

	std::string Ansi_To_Uft8(std::string wstrValue);

	bool IsTextUTF8(const char* str, UINT32 length);

	bool SpliteString(std::string strSrc,  std::string strDelim, std::vector<std::string>& vtStr);

	bool SpliteString(std::string strSrc, char cDelim, std::vector<std::string>& vtStr);

	bool ReplaceString(std::string& str, const std::string& pattern, const std::string& newpat);

	bool StringToVector(const char * pStrValue, INT32 IntVector[], INT32 nSize, char cDelim = ',');

	UINT32 VersionToInt(std::string& strVersion);

	INT32  CountSymbol(char* pStr, char cSymbol);

	bool StringTrim(std::string& strValue);

}



