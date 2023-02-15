#pragma once

#include <vector>
#include <cassert>

class DBInterface
{
private:
	DBInterface*			  Owner = nullptr;
	std::vector<DBInterface*> Components;

public:
	DBInterface*		 GetOwner();
	void				 SetOwner(DBInterface* NewOwner);
	virtual DBInterface* GetSystem();

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

	template <typename T> //
	T* GetComponent()
	{
		for (auto& Elem : Components)
		{
			T* OutValue = static_cast<T*>(Elem);
			if (OutValue)
			{
				return OutValue;
			}
		}
		return nullptr;
	}
};
