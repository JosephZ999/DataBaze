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

std::wstring DBPeopleData::ToWString()
{
	// return L"{ \"people\": [{\"id\": 1, \"name\":\"Human\",\"surname\":\"TAYLOR\"}, {\"id\": 2,
	// \"name\":\"TOM\",\"surname\":\"JERRY\"}]}";

	std::wstring Data;
	Data.append(L"{");
	JsonUtils::AddItem(Data, L"Name", Name).append(L",");
	JsonUtils::AddItem(Data, L"FamilyName", FamilyName);
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

	JsonUtils::OpenArray(Data, L"Main");
	for (size_t i = 0; i < 500; i++)
	{
		if (i > 0)
		{
			Data.append(L",");
		}

		JsonUtils::OpenArray(Data, std::wstring(L"Family ").append(std::to_wstring(i + 1)));
		for (size_t ii = 0; ii < 5; ++ii)
		{
			if (ii > 0)
			{
				Data.append(L",");
			}

			JsonUtils::OpenList(Data, std::wstring(L"List ").append(std::to_wstring(ii + 1)));
			JsonUtils::AddItem(Data,L"Item 1", L"Value 1").append(L",");
			JsonUtils::AddItem(Data,L"Item 2", L"Value 2").append(L",");
			JsonUtils::AddItem(Data,L"Item 3", L"Value 3").append(L",");
			JsonUtils::AddItem(Data,L"Item 4", L"Value 4").append(L",");
			JsonUtils::AddItem(Data,L"Item 5", L"Value 5").append(L",");
			JsonUtils::AddItem(Data,L"Item 6", L"Value 6");
			JsonUtils::CloseList(Data);
		}
		JsonUtils::CloseArray(Data);
	}
	JsonUtils::CloseArray(Data);

	return Data;

	Data.append(L"{ \"Family\": [");

	// Globals
	JsonUtils::OpenList(Data, L"Globals");
	JsonUtils::AddItem(Data, L"ChildrenNum", GetChildrenNum()).append(L",");
	JsonUtils::AddItem(Data, L"blablabla", L"hahaha");
	JsonUtils::CloseList(Data).append(L",");
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
