#include "CommonFunc.h"

#include <io.h>
#include <direct.h>
#include <time.h>

#ifdef _MSC_BUILD
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif


namespace ServerEngine
{
	uint32_t GetProcessorNum()
	{
		uint32_t dwNum = 0;
#ifdef _MSC_BUILD
		SYSTEM_INFO sysInfo;
		GetSystemInfo(&sysInfo);
		dwNum = sysInfo.dwNumberOfProcessors * 2;
#else
		dwNum = sysconf(_SC_NPROCESSORS_CONF);
#endif

		return dwNum;
	}

	std::string GetCurrentWorkDir()
	{
		char szPath[1024];

#ifdef _MSC_BUILD
		_getcwd(szPath, 1024);
#else
		getcwd(szPath, 1024);
#endif
		return std::string(szPath);
	}

	std::string GetCurrentExeDir()
	{
		char szPath[1024] = { 0 };
#ifdef _MSC_BUILD
		GetModuleFileNameA(NULL, szPath, 1024);
		char* p = strrchr(szPath, '\\');
#else
		readlink("/proc/self/exe", szPath, sizeof(szPath));
		char* p = strrchr(szPath, '/');
#endif
		* p = 0;
		return std::string(szPath);
	}


	bool SetCurrentWorkDir(std::string strPath)
	{
		if (strPath.empty())
		{
			strPath = GetCurrentExeDir();
		}

#ifdef _MSC_BUILD
		SetCurrentDirectoryA(strPath.c_str());
#else
		chdir(strPath.c_str());
#endif
		return true;
	}

	uint64_t GetCurrTime()
	{
		time_t t;

		t = time(0);

		return (UINT64)t;
	}

	tm GetCurrTmTime()
	{
		time_t rawtime;
		struct tm timeinfo;

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		return timeinfo;
	}

	uint64_t GetDayBeginTime()
	{
		time_t t;
		t = time(0);
		tm t_tm;
		localtime_s(&t_tm , &t);
		t_tm.tm_hour = 0;
		t_tm.tm_min = 0;
		t_tm.tm_sec = 0;
		t = mktime(&t_tm);
		return (uint64_t)t;
	}

	uint64_t GetWeekBeginTime()
	{
		time_t t;
		t = time(0);
		
		tm t_tm;
		localtime_s(&t_tm , &t);

		t_tm.tm_hour = 0;
		t_tm.tm_min = 0;
		t_tm.tm_sec = 0;
		t_tm.tm_wday = 0;
		t = mktime(&t_tm);
		return (UINT64)t;
	}

	time_t YearTimeToSec(int32_t nYear, int32_t nMonth, int32_t nDay, int32_t nHour, int32_t nMin, int32_t nSec)
	{
		time_t timer;
		time(&timer);

		tm t_tm;
		localtime_s(&t_tm , &timer);

		tm newtm;
		newtm.tm_year = (nYear < 0) ? t_tm.tm_year : nYear;
		newtm.tm_mon = (nMonth < 0) ? t_tm.tm_mon : nMonth;
		newtm.tm_mday = (nDay < 0) ? t_tm.tm_mday : nDay;
		newtm.tm_hour = (nHour < 0) ? t_tm.tm_hour : nHour;
		newtm.tm_min = (nMin < 0) ? t_tm.tm_min : nMin;
		newtm.tm_sec = (nSec < 0) ? t_tm.tm_sec : nSec;
		return mktime(&newtm);
	}

	uint64_t GetTickCountRefFunc()
	{
#ifdef _MSC_BUILD
		return ::GetTickCount64();
#else
		UINT64 uTickCount = 0;;
		struct timespec on;
		if (0 == clock_gettime(CLOCK_MONOTONIC, &on))
		{
			uTickCount = on.tv_sec * 1000 + on.tv_nsec / 1000000;
		}

		return uTickCount;
#endif
	}

	bool CreateDir(std::string& strDir)
	{
		int nRet = 0;
#ifdef _MSC_BUILD
		nRet = _mkdir(strDir.c_str());
#else
		nRet = mkdir(strDir.c_str(), S_IRWXU);
#endif

		if (nRet == 0)
		{
			return true;
		}

		if (errno == EEXIST)
		{
			return true;
		}

		return false;
	}

	bool GetDirFiles(const char* pszDir, char* pszFileType, std::vector<std::string>& vtFileList, bool bRecursion)
	{
		if (pszDir == NULL || pszFileType == NULL)
		{
			return false;
		}

		char   szTem[1024] = { 0 };
		char   szDir[1024] = { 0 };
		strcpy_s(szTem, 1024, pszDir);
		if (szTem[strlen(szTem) - 1] != '\\' || szTem[strlen(szTem) - 1] != '/')
		{
			strcat_s(szTem, "/");
		}

		strcpy_s(szDir, 1024, szTem);
		strcat_s(szDir, pszFileType);

#ifdef _MSC_BUILD
		struct _finddata_t  tFileInfo = { 0 };
		long long hFile = _findfirst(szDir, &tFileInfo);
		if (hFile == -1)
		{
			return false;
		}

		do
		{
			if (strcmp(tFileInfo.name, ".") == 0 || strcmp(tFileInfo.name, "..") == 0)
			{
				continue;
			}

			if ((tFileInfo.attrib & _A_SUBDIR) && bRecursion)
			{
				char   szSub[1024] = { 0 };
				strcpy_s(szSub, 1024, pszDir);
				if (szSub[strlen(szSub) - 1] != '\\' || szSub[strlen(szSub) - 1] != '/')
				{
					strcat_s(szSub, "/");
				}
				strcat_s(szSub, tFileInfo.name);
				GetDirFiles(szSub, pszFileType, vtFileList, bRecursion);
			}
			else
			{
				vtFileList.push_back(std::string(szTem) + std::string(tFileInfo.name));
			}
		} while (_findnext(hFile, &tFileInfo) == 0);
		_findclose(hFile);

#else

		DIR* pDirInfo;
		struct dirent* tFileInfo;
		struct stat statbuf;
		if ((pDirInfo = opendir(pszDir)) == NULL)
		{
			return false;
		}

		while ((tFileInfo = readdir(pDirInfo)) != NULL)
		{
			if (strcmp(".", tFileInfo->d_name) == 0 || strcmp("..", tFileInfo->d_name) == 0)
			{
				continue;
			}

			lstat(tFileInfo->d_name, &statbuf);
			if ((S_IFDIR & statbuf.st_mode) && bRecursion)
			{
				GetDirFiles(tFileInfo->d_name, pszFileType, vtFileList, bRecursion);
			}
			else
			{
				vtFileList.push_back(std::string(szTem) + std::string(tFileInfo->d_name));
			}
		}

		closedir(pDirInfo);
#endif
		return true;
	}

	bool IsSameDay(uint64_t uTime)
	{
#ifdef _MSC_BUILD
		long curTime = 0;
		_get_timezone(&curTime);
		return ((uTime - curTime) / 86400) == ((GetCurrTime() - curTime) / 86400);
#else
		return ((uTime - timezone) / 86400) == ((GetCurrTime() - timezone) / 86400);
#endif

	}

	uint32_t GetCurThreadID()
	{
		uint32_t dwThreadID = 0;
#ifdef _MSC_BUILD
		dwThreadID = ::GetCurrentThreadId();
#else
		dwThreadID = (UINT32)pthread_self();
#endif
		return dwThreadID;
	}

	uint32_t GetCurProcessID()
	{
		uint32_t dwProcessID = 0;
#ifdef _MSC_BUILD
		dwProcessID = ::GetCurrentProcessId();
#else
		dwProcessID = (UINT32)getpid();
#endif
		return dwProcessID;
	}


	void Sleep(uint32_t dwMilliseconds)
	{
#ifdef _MSC_BUILD
		::Sleep(dwMilliseconds);
#else
		struct timespec req;
		req.tv_sec = 0;
		req.tv_nsec = dwMilliseconds * 1000000;
		nanosleep(&req, NULL);
#endif
		return;
	}

	uint32_t GetFreePhysMemory()
	{
		uint32_t dwFreeSize = 0;
#ifdef _MSC_BUILD
		MEMORYSTATUSEX statex;

		statex.dwLength = sizeof(statex);

		GlobalMemoryStatusEx(&statex);

		dwFreeSize = (uint32_t)(statex.ullAvailPhys / 1024 / 1024);
#else
		UINT32 dwPageSize;
		UINT32 dwFreePages;
		dwPageSize = sysconf(_SC_PAGESIZE) / 1024;
		dwFreePages = sysconf(_SC_AVPHYS_PAGES) / 1024;
		dwFreeSize = dwFreePages * dwPageSize;
#endif

		return dwFreeSize;
	}

	int32_t GetRandNum(int32_t nType)
	{
		if (nType >= 100 || nType < 0)
		{
			return 0;
		}

		static int32_t nRandIndex[100] = { 0 };
		static int32_t vtGlobalRankValue[10000];
		static bool  bInit = false;

		if (bInit == false)
		{
			bInit = true;
			int32_t  nTempIndex;
			uint32_t nTemp;
			for (int j = 0; j < 10000; j++)
			{
				vtGlobalRankValue[j] = j + 1;
			}

			for (int i = 0; i < 10000; i++)
			{
				nTempIndex = rand() % (i + 1);
				if (nTempIndex != i)
				{
					nTemp = vtGlobalRankValue[i];
					vtGlobalRankValue[i] = vtGlobalRankValue[nTempIndex];
					vtGlobalRankValue[nTempIndex] = nTemp;
				}
			}
		}

		return  vtGlobalRankValue[(nRandIndex[nType]++) % 10000];
	}

	UINT32 GetLastErrorWrapFunc()
	{
#ifdef _MSC_BUILD
		return ::GetLastError();
#else
		return errno;
#endif
	}
			
	bool DbgTrace(wchar_t* format, ...)
	{
#if (defined _MSC_BUILD) && (defined _DEBUG)
		wchar_t szLog[1024] = { 0 };
		va_list argptr;
		va_start(argptr, format);
		_vsnwprintf_s(szLog, 1023, format, argptr);
		va_end(argptr);
		OutputDebugString(szLog);
#endif

		return true;
	}

	bool KillProcess(uint64_t dwPid)
	{
#ifdef _MSC_BUILD
		HANDLE hPrc;
		if (0 == dwPid)
		{
			return false;
		}

		hPrc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)dwPid);
		if (hPrc == NULL)
		{
			return true;
		}

		if (!TerminateProcess(hPrc, 0))
		{
			CloseHandle(hPrc);
			return false;
		}
		else
		{
			WaitForSingleObject(hPrc, 200);
		}
		CloseHandle(hPrc);
#else
		kill(dwPid, SIGKILL);
#endif
		return true;
	}

	int32_t Min(int32_t nValue1, int32_t nValue2)
	{
		return (nValue1 < nValue2) ? nValue1 : nValue2;
	}
}