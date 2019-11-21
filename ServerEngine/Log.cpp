﻿#include "Platform.h"
#include "CommonFunc.h"
#include "Log.h"

CLog::CLog(void)
{
	m_LogLevel = 0;
}

CLog::~CLog(void)
{
	CloseLog();
}

CLog* CLog::GetInstancePtr()
{
	static CLog Log;

	return &Log;
}

bool CLog::StartLog(std::string strPrefix, std::string strLogDir)
{
	if (!ServerEngine::CreateDir(strLogDir))
	{
		return false;
	}
	m_strPrefix = strPrefix;
//#ifdef _MSC_BUILD
//	::SetConsoleMode(::GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS);
//#endif
	m_pLogFile = NULL;

	tm CurTime = ServerEngine::GetCurrTmTime();

	char szFileName[512];

	snprintf(szFileName, 512, "%s/%s-%02d%02d%02d-%02d%02d%02d.log", strLogDir.c_str(), strPrefix.c_str(), CurTime.tm_year, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec);

	auto err = fopen_s(&m_pLogFile, szFileName, "w+");

	if (err != 0)
	{
		return false;
	}

	m_LogCount = 0;

	return true;
}

bool CLog::CloseLog()
{
	if (m_pLogFile == NULL)
	{
		return false;
	}

	fflush(m_pLogFile);

	fclose(m_pLogFile);

	m_pLogFile = NULL;

	return true;
}

void CLog::LogWarnning(char* lpszFormat, ...)
{
	if (m_LogLevel >= Log_Info)
	{
		return;
	}

	if (m_pLogFile == NULL)
	{
		return;
	}

	tm CurTime = ServerEngine::GetCurrTmTime();

	char szLog[512];
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & ServerEngine::GetCurThreadID());

	va_list argList;
	va_start(argList, lpszFormat);
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(), lpszFormat, argList);
	va_end(argList);

	strncat_s(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();

	return;
}

void CLog::LogError(const char* lpszFormat, ...)
{
	if (m_LogLevel >= Log_Error)
	{
		return;
	}

	if (m_pLogFile == NULL)
	{
		return;
	}

	char szLog[512];
	tm CurTime = ServerEngine::GetCurrTmTime();
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & ServerEngine::GetCurThreadID());

	va_list argList;
	va_start(argList, lpszFormat);
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(), lpszFormat, argList);
	va_end(argList);

	strncat_s(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();

	return;
}

void CLog::LogInfo(const char* lpszFormat, ...)
{
	if (m_LogLevel >= Log_Info)
	{
		return;
	}

	if (m_pLogFile == NULL)
	{
		return;
	}

	char szLog[512];

	tm CurTime = ServerEngine::GetCurrTmTime();
	snprintf(szLog, 512, "[%s][%02d-%02d-%02d %02d:%02d:%02d][%04x] ", m_strPrefix.c_str(), CurTime.tm_year % 100, CurTime.tm_mon + 1, CurTime.tm_mday, CurTime.tm_hour, CurTime.tm_min, CurTime.tm_sec, 0xffff & ServerEngine::GetCurThreadID());

	va_list argList;
	va_start(argList, lpszFormat);
	vsnprintf(szLog + 28 + m_strPrefix.size(), 512 - 28 - m_strPrefix.size(), lpszFormat, argList);
	va_end(argList);

	strncat_s(szLog, "\n", 10);

	m_CritSec.Lock();
	fputs(szLog, m_pLogFile);
	m_LogCount++;
	printf(szLog);
	m_CritSec.Unlock();


	return;
}

void CLog::SetLogLevel(int Level)
{
	m_LogLevel = Level;

	return;
}

void CLog::Flush()
{
	if (m_LogCount > 0)
	{
		fflush(m_pLogFile);

		m_LogCount = 0;
	}
}
