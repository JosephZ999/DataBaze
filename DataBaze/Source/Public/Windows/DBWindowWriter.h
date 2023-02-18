#pragma once
#include "framework.h"
#include "resource.h"
#include "DBInterface.h"
#include "DBDataTypes.h"

#include "DBDelegate.h"

DECLARE_DELEGATE(OnWriteSuccessSignature);

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
	~DBWindowWriter();

	OnWriteSuccessSignature OnWriteSuccess;

	HWND WindowHandle = 0;

	DBFamilyData MembersData;

	// Writing progress
	DBPeopleData* DataToChange = nullptr;

	EPeopleType		PeopleType		   = EPeopleType::PT_Parent;
	EPeopleData		PeopleData		   = EPeopleData::PD_Name;
	EMeritialStatus Status			   = EMeritialStatus::MS_Unmarried;
	size_t			ChildrenNum		   = 0;
	size_t			EnteredChildrenNum = 0;
	std::wstring	ImagePath;
	bool			Finish = false;

public:
	void OnConstruct();
	bool CheckFormat();
	void WriteData();
	void SelectWriteData(EPeopleType PT);

private:
	void SetEditboxStyle(LONG Style = 0, int TextLimit = 0);
	void SetItem(std::string& Info);

	void UpdateInfo();
	void SelectChild(size_t Index);

	void UpdateEditStyle();

	void NextPeople();
	void NextLine();
	void OpenImage();
	bool CopyImage();

	void FinishWriting();
};
