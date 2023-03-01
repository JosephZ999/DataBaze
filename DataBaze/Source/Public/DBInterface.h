#pragma once

#include <vector>
#include <cassert>
#include "Core/DBCommandCenter.h"

class DBInterface
{

private:
	DBInterface*			  Owner = nullptr;
	std::vector<DBInterface*> Components;

public:
	DBInterface*		 GetOwner() const;
	void				 SetOwner(DBInterface* NewOwner);

protected:
	virtual ~DBInterface();

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
