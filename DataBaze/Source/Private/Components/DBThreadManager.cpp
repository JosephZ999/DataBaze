#include "DBThreadManager.h"
#include "DBThreadTypes.h"
#include <Windows.h>

void DBThreadManager::NewThread(DBThreadUser* OuterObj, ThreadFuncType ThreadFunction)
{
	// Check OuterObject
	// ...

	HANDLE NewThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
	Threads.push_back(DBThreadData(OuterObj, NewThread));
}
