#pragma once

#include "Platform.h"

#include <string>
#include <vector>
#include <cstdint>

namespace ServerEngine
{
	int32_t StringToInt(char* pStr);

	int32_t StringToInt(const char* pStr);

	int64_t StringToInt64(char* pStr);

	int64_t StringToInt64(const char* pStr);

	float StringToFloat(char* pStr);

	float StringToFloat(const char* pStr);

	DOUBLE StringToDouble(char* pStr);

	std::string DoubleToString(double dValue);

	std::string IntToString(uint32_t nValue);

	std::string IntToString(int32_t nValue);

	std::string IntToString(uint64_t nValue);

	std::string IntToString(int64_t nValue);

	bool  StringToPos(char* pStr, float& x, float& y, float& z);

	//浮点到字符串， nPrecision 保留的最大小数的位数， bRound 是否四舍五入
	std::string FloatToString(float fValue, int32_t nPrecision = -1, bool bRound = false);

	std::wstring Utf8_To_Unicode(std::string strValue);

	std::string  Unicode_To_Uft8(std::wstring wstrValue);

	std::wstring Ansi_To_Unicode(std::string strValue);

	std::string Unicode_To_Ansi(std::wstring strValue);

	std::string Utf8_To_Ansi(std::string strValue);

	std::string Ansi_To_Uft8(std::string wstrValue);

	bool IsTextUTF8(const char* str, uint32_t length);

	bool SpliteString(std::string strSrc,  std::string strDelim, std::vector<std::string>& vtStr);

	bool SpliteString(std::string strSrc, char cDelim, std::vector<std::string>& vtStr);

	bool ReplaceString(std::string& str, const std::string& pattern, const std::string& newpat);

	bool StringToVector(const char * pStrValue, int32_t IntVector[], int32_t nSize, char cDelim = ',');

	uint32_t VersionToInt(std::string& strVersion);

	int32_t  CountSymbol(char* pStr, char cSymbol);

	bool StringTrim(std::string& strValue);

}



