#include "DBInterface.h"

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
	if (Owner)
	{
		return Owner->GetSystem();
	}
	return nullptr;
}
