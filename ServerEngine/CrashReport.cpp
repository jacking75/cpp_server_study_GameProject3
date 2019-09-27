﻿#ifdef _MSC_BUILD
#include <string>

#pragma warning(disable: 4996)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <stdio.h>
#include <Dbghelp.h>

#include "crashReport.h"
#include "time.h"

#pragma auto_inline (off)
#pragma comment( lib, "DbgHelp" )


LPTOP_LEVEL_EXCEPTION_FILTER g_preFilter;

std::string g_AppName;


long   __stdcall  CrashCallBack(_EXCEPTION_POINTERS* pExInfo)
{
	struct tm* pTime;
	time_t ctTime;
	time(&ctTime);
	pTime = localtime( &ctTime );
	char tem[256] = { 0, };
	snprintf(tem, 256, ("%s-%d-%d-%d_%d-%d-%d.dmp"), g_AppName.c_str(),
	         1900 + pTime->tm_year, 1 + pTime->tm_mon, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);

	HANDLE hFile = ::CreateFileA(tem, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION einfo;
		einfo.ThreadId = ::GetCurrentThreadId();
		einfo.ExceptionPointers = pExInfo;
		einfo.ClientPointers = false;
		::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hFile, MiniDumpNormal, &einfo, NULL, NULL);
		::CloseHandle(hFile);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

void  SetCrashReport(std::string strAppName)
{
	g_AppName = strAppName;
	g_preFilter = SetUnhandledExceptionFilter(CrashCallBack);
}

void UnSetCrashReport()
{
	SetUnhandledExceptionFilter(g_preFilter);

	return;
}


#else
void  SetCrashReport(std::string strAppName)
{
	return ;
}

void UnSetCrashReport()
{
	return;
}

#endif