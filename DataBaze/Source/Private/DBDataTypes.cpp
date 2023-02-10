#include "DBDataTypes.h"

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

std::wstring DBPeopleData::ToWString()
{
	// return L"{ \"people\": [{\"id\": 1, \"name\":\"Human\",\"surname\":\"TAYLOR\"}, {\"id\": 2,
	// \"name\":\"TOM\",\"surname\":\"JERRY\"}]}";

	std::wstring Data;
	Data.append(L"{");

	Data.append(L"\"").append(L"Name").append(L"\":");
	Data.append(L"\"").append(Name).append(L"\",");

	Data.append(L"\"").append(L"FamilyName").append(L"\":");
	Data.append(L"\"").append(FamilyName).append(L"\"");

	Data.append(L"}");
	return Data;
}

std::wstring DBFamilyData::ToWString()
{
	// return L"{ \"people\": [{\"id\": 1, \"name\":\"Human\",\"surname\":\"TAYLOR\"}, {\"id\": 2,
	// \"name\":\"TOM\",\"surname\":\"JERRY\"}]}";

	const bool HasChild	 = Children.size() > 0;
	const bool HasSpouse = Parents.size() > 1;

	std::wstring Data;
	Data.append(L"{ \"Family\": [");

	// Globals
	Data.append(L"{ \"Globals\": {");

	// Data.append(L"{").append(L"\"ChildrenNum\" :").append(std::to_wstring(GetChildrenNum())).append(L"},");
	// Data.append(L"{").append(L"\"blablabla\" :").append(L"\"hahaha\"").append(L"}");

	Data.append(L"\"ChildrenNum\" :").append(std::to_wstring(GetChildrenNum())).append(L",");
	Data.append(L"\"blablabla\" :").append(L"\"hahaha\"");

	Data.append(L"}},");
	// End Globals

	// Parents
	Data.append(L"{ \"Parent_1\": ");
	Data.append(Parents[0].ToWString());
	if (HasSpouse)
	{
		Data.append(L"}, { \"Parent_2\": ");
		Data.append(Parents[1].ToWString());
	}
	Data.append(L"}");
	// End Parents

	// Children
	if (HasChild)
	{
		Data.append(L",");

		int ChildIndex = 0;
		for (auto& Elem : Children)
		{
			if (ChildIndex > 0)
			{
				Data.append(L",");
			}
			++ChildIndex;

			Data.append(L"{ \"Child_").append(std::to_wstring(ChildIndex)).append(L"\": ");
			Data.append(Elem.ToWString());
			Data.append(L"}");
		}
	}
	// End Children

	Data.append(L"]}");

	return Data;
}

int DBFamilyData::GetChildrenNum()
{
	return Children.size();
}
