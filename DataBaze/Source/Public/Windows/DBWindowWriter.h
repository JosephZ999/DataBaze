#pragma once
#include "framework.h"
#include "resource.h"
#include "DBInterface.h"
#include "DBDataTypes.h"

#include "DBDelegate.h"

class DBButtonManager;

DECLARE_DELEGATE(OnWriteSuccessSignature);

struct FImagePath
{
	std::wstring Initial;
	std::wstring Final;
	int			 ImageId;
};

class DBWindowWriter : public DBInterface
{
	typedef std::vector<FImagePath> Images;

public:
	DBWindowWriter(DBInterface* InOwner);

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
	bool			Finish			   = false;

	Images ImagesToCopy;
	int	   LastImageId = 1;

private:
	DBButtonManager* ButtonManager = nullptr;

public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

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

	bool SaveImage(const std::wstring& InImagePath);
	void CopySavedImages();
	int	 GetNextImageId() const;

	void FinishWriting();
};
