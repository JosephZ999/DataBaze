#include "DBDataTypes.h"
#include "DBJsonUtils.h"

void ButtonContainer::Add(const DBWindow& InButton)
{
	if (ButtonLastIndex < 10)
	{
		Buttons[ButtonLastIndex] = InButton;
		++ButtonLastIndex;
		return;
	}
}

bool ButtonContainer::FindByIndex(EDBWinCompId Id, DBWindow& OutWindow) const
{
	for (auto& Elem : Buttons)
	{
		if (Elem.Id == Id)
		{
			OutWindow = Elem;
			return true;
		}
	}
	return false;
}

bool ButtonContainer::Contains(EDBWinCompId Id) const
{
	for (auto& Elem : Buttons)
	{
		if (Elem.Id == Id)
		{
			return true;
		}
	}
	return false;
}

bool DBFamilyData::IsHasChildren() const
{
	return Children.size() > 0;
}

bool DBFamilyData::IsHasASpouse() const
{
	return Parents.size() > 1;
}

EMeritialStatus DBFamilyData::GetStatus() const
{
	return EMeritialStatus(static_cast<EMeritialStatus>(MaritalStatus));
}

void DBFamilyData::SwitchParents()
{
	auto Cache = Parents[0];
	Parents[0] = Parents[1];
	Parents[1] = Cache;
}

EEducationDegree DBPeopleData::GetEducation() const
{
	return static_cast<EEducationDegree>(EducationDegree);
}
