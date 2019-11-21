#pragma once

#include <string>
#include <vector>

#include "Platform.h"

#include <cstdint>


#define GET_BIT(X,Y) (((X) >> (Y-1)) & 1)
#define SET_BIT(X,Y) ((X) |= (1 << (Y)))
#define CLR_BIT(X,Y) ((X) &= (~(1<<Y)))

namespace ServerEngine
{
uint32_t			GetProcessorNum();

std::string		GetCurrentWorkDir();

bool			SetCurrentWorkDir(std::string strPath);

std::string		GetCurrentExeDir();

bool			CreateDir(std::string& strDir);

bool			GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, bool bRecursion);

bool			IsSameDay(uint64_t uTime);

uint64_t			GetCurrTime(); //获取当前的秒数

tm				GetCurrTmTime();

uint64_t			GetDayBeginTime(); //获取当前0点的秒数

uint64_t			GetWeekBeginTime(); //获取当前0点的秒数

time_t			YearTimeToSec(int32_t nYear, int32_t nMonth, int32_t nDay, int32_t nHour, int32_t nMin, int32_t nSec);

uint64_t			GetTickCountRefFunc();

uint32_t			GetCurThreadID();

uint32_t			GetCurProcessID();

void			Sleep(uint32_t dwMilliseconds);

uint32_t			GetFreePhysMemory();
 
int32_t           GetRandNum(int32_t nType);

uint32_t			GetLastErrorWrapFunc();

bool			DbgTrace(char* format, ...);

bool			KillProcess(uint64_t dwPid);

int32_t			Min(int32_t nValue1, int32_t nValue2);
}


