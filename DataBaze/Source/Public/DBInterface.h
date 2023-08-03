#pragma once

#include <vector>
#include <cassert>
#include "Core/DBCommandCenter.h"
#include "DBMacroUtils.h"

/*
 * Its Component
 * Has Owner
 * Can Create SubObjects
 * Can Remove All SubObjects
 */
class DBInterface
{
private:
	DBInterface*			  Owner = nullptr;
	std::vector<DBInterface*> Components;

protected:
	bool PendingDestroy = false;

public:
	virtual ~DBInterface();

	DBInterface* GetOwner() const;
	void		 SetOwner(DBInterface* NewOwner);
	virtual bool CanBeDestroyed() const;
	bool		 IsPendingDestroy() const { return PendingDestroy; }
	virtual void Destroy();

protected:
	template <typename T> //
	T* CreateComponent()
	{
		T* NewComp = new T;
		if (auto NewInterface = static_cast<DBInterface*>(NewComp))
		{
			NewInterface->SetOwner(this);
			Components.push_back(NewInterface);
			return NewComp;
		}
		else if (NewComp)
		{
			delete NewComp;
		}
		return nullptr;
	}

	void RemoveAllComponents();

public:
	template <typename T> //
	T* GetComponent() const
	{
		for (auto Elem : Components)
		{
			T* OutValue = dynamic_cast<T*>(Elem);
			if (OutValue)
			{
				return OutValue;
			}
		}
		return nullptr;
	}
};
