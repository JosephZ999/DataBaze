#include "DBThreadManager.h"
#include "DBThreadTypes.h"
#include <Windows.h>

void DBThreadManager::NewThread(DBThreadUser* Client, ThreadFuncType ThreadFunction)
{
	if (! Client) return;

	bool ThreadAlreadyCreated = false;
	for (auto& ThreadData : Threads)
	{
		if (ThreadData.Object == Client)
		{
			ThreadAlreadyCreated = true;
		}
	}

	if (! ThreadAlreadyCreated) // for this obj
	{
		Client->Enabled	 = true;
		HANDLE NewThread = CreateThread(NULL, 0, ThreadFunction, NULL, 0, NULL);
		Threads.push_back(DBThreadData(Client, NewThread));
	}
}

void DBThreadManager::FinishThread(DBThreadUser* Client)
{
	for (size_t i = 0; i < Threads.size(); ++i)
	{
		if (Threads[i].Object == Client)
		{
			Client->Enabled = false;
			Threads.erase(Threads.begin() + i);
		}
	}
}
