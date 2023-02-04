#pragma once
#include "framework.h"
#include "resource.h"
#include "DBInterface.h"
#include "DBDataTypes.h"

LRESULT CALLBACK WndWriterProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowWriter : public DBInterface
{

public:
	DBWindowWriter() {}
	DBWindowWriter(DBInterface* InOwner);
	DBWindowWriter(HWND InOwnerHWND)
		: WindowHandle(InOwnerHWND)
	{
	}

	HWND WindowHandle = 0;

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
	void OpenImage();
	bool CopyImage();

	std::wstring ImagePath;
};
