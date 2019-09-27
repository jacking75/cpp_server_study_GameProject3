#pragma once

#include <string>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Platform.h"


#define GET_BIT(X,Y) (((X) >> (Y-1)) & 1)
#define SET_BIT(X,Y) ((X) |= (1 << (Y)))
#define CLR_BIT(X,Y) ((X) &= (~(1<<Y)))

namespace ServerEngine
{
UINT32			GetProcessorNum();

std::string		GetCurrentWorkDir();

bool			SetCurrentWorkDir(std::string strPath);

std::string		GetCurrentExeDir();

bool			CreateDir(std::string& strDir);

bool			GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, bool bRecursion);

bool			IsSameDay(UINT64 uTime);

UINT64			GetCurrTime(); //获取当前的秒数

tm				GetCurrTmTime();

UINT64			GetDayBeginTime(); //获取当前0点的秒数

UINT64			GetWeekBeginTime(); //获取当前0点的秒数

time_t			YearTimeToSec(INT32 nYear, INT32 nMonth, INT32 nDay, INT32 nHour, INT32 nMin, INT32 nSec);

UINT64			GetTickCountRefFunc();

UINT32			GetCurThreadID();

UINT32			GetCurProcessID();

void			Sleep(UINT32 dwMilliseconds);

UINT32			GetFreePhysMemory();
 
INT32           GetRandNum(INT32 nType);

UINT32			GetLastErrorWrapFunc();

// HANDLE       CreateShareMemory(std::string strName, INT32 nSize);
//
// HANDLE		OpenShareMemory(std::string strName);

HANDLE          CreateShareMemory(UINT32 dwModuleID, INT32 nPage, INT32 nSize);

HANDLE			OpenShareMemory(UINT32 dwModuleID, INT32 nPage);

CHAR* 			GetShareMemory(HANDLE hShm);

bool 			ReleaseShareMemory(CHAR* pMem);

bool			CloseShareMemory(HANDLE hShm);

bool			DbgTrace(char* format, ...);

bool			KillProcess(UINT64 dwPid);

INT32			Min(INT32 nValue1, INT32 nValue2);
}


