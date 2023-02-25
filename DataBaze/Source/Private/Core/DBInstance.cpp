#include "DBInstance.h"

void DBInstance::Initialize(FDBInstanceInit& Param)
{
	if (Initialized) return;

	InitData	= Param;

	Initialized = true;
}

FDBInstanceInit DBInstance::GetInitData() const
{
	return InitData;
}
