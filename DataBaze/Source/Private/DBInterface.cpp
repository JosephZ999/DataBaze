#include "DBInterface.h"

DBInterface* DBInterface::GetOwner() const
{
	return Owner;
}

void DBInterface::SetOwner(DBInterface* NewOwner)
{
	Owner = NewOwner;
}

DBInterface::~DBInterface()
{
	RemoveAllComponents();
}

void DBInterface::RemoveAllComponents()
{
	for (auto& Elem : Components)
	{
		delete Elem;
	}
	Components.clear();
}
