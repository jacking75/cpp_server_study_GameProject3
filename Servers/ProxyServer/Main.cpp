#include "GameService.h"
#include "..\..\ServerEngine\CrashReport.h"
#include "..\..\ServerEngine\CommandLine.h"

#include <Windows.h>

#include <string>


int main(int argc, char* argv[])
{
	SetCrashReport("ProxyServer");

	// --Index=1 --MaxConnCount=8 --Port=32452
	ServerEngine::CCommandLine cmdLine(argc, argv);

	ServerConfig serverConfig;
	serverConfig.Index = cmdLine.GetIntValue("--Index");
	serverConfig.MaxConnCount = cmdLine.GetIntValue("--MaxConnCount");
	serverConfig.Port = cmdLine.GetIntValue("--Port");
	
	CGameService::GetInstancePtr()->SetWatchIndex(serverConfig.Index);

	if (CGameService::GetInstancePtr()->Init(serverConfig) == false)
	{
		return 0;
	}

	CGameService::GetInstancePtr()->Run();

	CGameService::GetInstancePtr()->Uninit();

	UnSetCrashReport();

	return 0;
}

