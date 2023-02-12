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

bool ButtonContainer::FindByIndex(EDBWinCompId Id, DBWindow& Button)
{
	for (int i = 0; i < 10; ++i)
	{
		if (Buttons[i].Id != Id) continue;

		Button = Buttons[i];
		return true;
	}

	return false;
}

int DBFamilyData::GetChildrenNum() const
{
	return Children.size();
}

bool DBFamilyData::IsHasChildren() const
{
	return Children.size() > 0;
}

bool DBFamilyData::IsHasASpouse() const
{
	return Parents.size() > 1;
}
