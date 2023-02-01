#pragma once
#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

LRESULT CALLBACK WndWriterProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowWriter
{
	DBWindowWriter() {}

public:
	DBWindowWriter(HWND InOwnerHWND)
		: OwnerHWND(InOwnerHWND)
	{
	}

	HWND OwnerHWND = 0;

	DBFamilyData MembersData;

	// Writing progress
	DBPeopleData* DataToChange = nullptr;

	EPeopleType PeopleType = EPeopleType::PT_Parent;
	EPeopleData PeopleData = EPeopleData::PD_Name;

	void SelectWriteData(EPeopleType PT);
	void WriteData();
	void UpdateInfo();
	void SelectChild(size_t Index);
	void SetInfoText(std::wstring& Text);
	bool GetLineOfData(std::wstring*& OutData, EPeopleData DataType);
	void NextPeople();
	void NextLine();
};
