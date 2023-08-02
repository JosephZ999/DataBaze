#pragma once
#include "DBInterface.h"
#include <vector>

typedef LPTHREAD_START_ROUTINE ThreadFuncType;

class DBThreadUser;

struct DBThreadData
{
	DBThreadData(DBThreadUser* InObj, HANDLE InH)
		: Object(InObj)
		, ThreadHandle(InH)
	{}

	DBThreadUser* Object;
	HANDLE		  ThreadHandle;
};

class DBThreadManager : public DBInterface
{
private:
	std::vector<DBThreadData> Threads;

public:
	void NewThread(DBThreadUser* Client, ThreadFuncType ThreadFunction);
	void FinishThread(DBThreadUser* Client);
};
