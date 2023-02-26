#include "DBInstance.h"
#include "Components/DBDataManager.h"
#include "Components/DBWindowsManager.h"

DBInstance::DBInstance()
{
	WindowManager = CreateComponent<DBWindowsManager>();
	DataManager	  = CreateComponent<DBDataManager>();

	assert(WindowManager);
	assert(DataManager);
}

void DBInstance::Initialize(FDBInstanceInit& Param)
{
	if (Initialized) return;

	InitData = Param;

	Initialized = true;
}

FDBInstanceInit DBInstance::GetInitData() const
{
	return InitData;
}
