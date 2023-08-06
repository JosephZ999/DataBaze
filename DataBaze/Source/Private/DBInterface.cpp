#include "DBInterface.h"

DBInterface::~DBInterface()
{
	Destroy();
}

DBInterface* DBInterface::GetOwner() const
{
	return Owner;
}

void DBInterface::SetOwner(DBInterface* NewOwner)
{
	Owner = NewOwner;
}

bool DBInterface::CanBeDestroyed() const
{
	bool AllClear = true;
	for (auto& Elem : Components)
	{
		if (! Elem->IsPendingDestroy())
		{
			AllClear = false;
		}
	}
	return AllClear;
}

void DBInterface::RemoveAllComponents()
{
	bool AllClear = true;
	for (auto& Elem : Components)
	{
		Elem->Destroy();
		if (! Elem->IsPendingDestroy())
		{
			AllClear = false;
		}
	}
	if (AllClear)
	{
		for (auto& Elem : Components)
		{
			delete Elem;
		}
		Components.clear();
	}
}

void DBInterface::Destroy()
{
	RemoveAllComponents();
	PendingDestroy = CanBeDestroyed();
}
