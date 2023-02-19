#include "DBInterface.h"

DBInterface* DBInterface::GetOwner() const
{
	return Owner;
}

void DBInterface::SetOwner(DBInterface* NewOwner)
{
	Owner = NewOwner;
}

DBInterface* DBInterface::GetSystem() const
{
	if (Owner)
	{
		return Owner->GetSystem();
	}
	return nullptr;
}
