// ProxyServer.cpp : 定义控制台应用程序的入口点。
//
#include <string>

#include <Windows.h>

#include "GameService.h"
#include "..\..\ServerEngine\CrashReport.h"
#include "..\..\ServerEngine\CommandLine.h"

int main(int argc, char* argv[])
{
	SetCrashReport("ProxyServer");

	ServerEngine::CCommandLine cmdLine(argc, argv);

	CGameService::GetInstancePtr()->SetWatchIndex(cmdLine.GetIntValue("windex"));

	if (!CGameService::GetInstancePtr()->Init())
	{
		return 0;
	}

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	UnSetCrashReport();

	return 0;
}

