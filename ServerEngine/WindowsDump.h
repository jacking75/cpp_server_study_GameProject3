#pragma once

#ifdef _MSC_BUILD
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <dbghelp.h>

namespace ServerEngine
{
#define DUMP_FILE L".\\WindowsP.dmp"

	void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS* pException)
	{
		// 덤프 파일 생성  
		HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hDumpFile == INVALID_HANDLE_VALUE)
		{
			return;
		}

		// Dump 정보  
		MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = true;

		// Dump 파일 내용 쓰기 
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

		CloseHandle(hDumpFile);

		return;
	}

	LONG ApplicationCrashHandler(EXCEPTION_POINTERS* pException)
	{
		CreateDumpFile(DUMP_FILE, pException);

		FatalAppExit(-1, L"*** Unhandled Exception! ***");

		return EXCEPTION_EXECUTE_HANDLER;
	}

	//예
	/*
		 SetUnhandledExceptionFilter(ApplicationCrashHandler);
	*/

#endif

}