#include "DBInterface.h"

DBInterface::DBInterface()
{
	Owner = nullptr;
	// Components.resize(10);
}

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
