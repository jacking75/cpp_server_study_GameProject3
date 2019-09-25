#pragma once

#ifndef _MSC_BUILD
#include <pthread.h>
typedef pthread_t THANDLE;
#define Th_RetName void*
#define Th_RetValue NULL
#else
#include "Platform.h"
typedef HANDLE THANDLE;
#define Th_RetName void
#define Th_RetValue
#endif

typedef Th_RetName* ThreadFunc( void* pParam );

namespace CommonThreadFunc
{
THANDLE		CreateThread( Th_RetName (*pThreadFunc)(void*),  void* pArg);

VOID		ExitThread();

BOOL		WaitThreadExit(THANDLE hThread);
}

