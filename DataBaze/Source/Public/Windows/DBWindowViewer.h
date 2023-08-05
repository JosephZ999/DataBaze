#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"
#include "DBDelegate.h"
#include <memory>

class DBButtonManager;
class DBAutofill;

enum EAutoFillStep
{
	AFS_None,
	AFS_Part1,
	AFS_Part2,
	AFS_Check,
	AFS_SaveResult,
	AFS_Max,
};

DECLARE_DELEGATE(OnViewerCloseSignature);

struct FillData
{
	std::wstring Title;
	std::string	 Info;
};

class DBWindowViewer : public DBInterface
{
public:
	DBWindowViewer(HWND OwningWnd);
	OnViewerCloseSignature OnClose;

	HWND		 WindowHandle = 0;
	DBFamilyData MemberData;

private:
	std::shared_ptr<DBAutofill> AutoFillObj;
	DBButtonManager* ButtonManager;
	bool			 HasSpouse = false;
	bool			 HasChild  = false;
	std::wstring	 Title;
	std::string		 Info;
	FMemberId		 MemberId;
	int				 ListItemId = 0;

	EPeopleType CurrentPeople = PT_Parent;
public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetMemberData(FMemberId InId, const DBFamilyData& InData);

private:
	void ChangePeople(bool Next);
	void PrintData();
	void PrintPeople(const DBPeopleData& People, bool IsChild);
	void PrintMail();

	void OnFillFinish();
};
