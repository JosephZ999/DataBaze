#include "..\Public\DBInterface.h"

DBInterface* DBInterface::GetOwner()
{
	return Owner;
}

void DBInterface::SetOwner(DBInterface* NewOwner)
{
	Owner = NewOwner;
}

DBInterface* DBInterface::GetSystem()
{
	return nullptr;
}
