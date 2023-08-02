#include "DBAutofill.h"
#include "DBThreadManager.h"
#include "DBCommandCenter.h"
#include <assert.h>

DBAutofill* Object = nullptr;

DWORD WINAPI ThreadFunc(LPVOID Param)
{
	while (Object && Object->IsThreadRunning())
	{
		//
	}
	return 0;
}

DBAutofill::DBAutofill()
{
	assert(! Object);

	Object = this;

	if (auto ThreadManager = cmd::get::GetThreadManager())
	{
		ThreadManager->NewThread(this, ThreadFunc);
	}
}

DBAutofill::~DBAutofill()
{
	if (Object && this == Object)
	{
		if (auto ThreadManager = cmd::get::GetThreadManager())
		{
			ThreadManager->FinishThread(this); // can you past it to DBThreadUser class
		}
		Object = nullptr;
	}
}
