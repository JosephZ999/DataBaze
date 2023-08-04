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

	std::vector<EAutoFillStep> StepMap;
	EAutoFillStep			   CurrentStep = EAutoFillStep::AFS_None;

	bool SecondFormAvailable = false;

	bool FillingInProgress = false;

public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void SetMemberData(FMemberId InId, const DBFamilyData& InData);

private:
	void InitializeSteps();

	void AutoFill();
	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Check();
	void Autofill_SaveResult();

	/*
	 * Will fill people:
	 * name
	 * family name
	 * Gender
	 * birth data
	 * country where was born
	 */
	void FillPeopleData(const DBPeopleData& InPeople, bool bPartOne);

	void ChangePeople(bool Next);

	void PrintData();
	void PrintPeople(const DBPeopleData& People, bool IsChild);
	void PrintMail();
	void PressTab(size_t Times = 1);

	void CopyAndSaveCode();

	inline void PasteString(const std::string Text);
	inline void WriteEMail();

	void PasteImagePath(const DBPeopleData& People);
	void WriteString(std::string Text);

	void PasteMailInfo();
	void SelectEducationDegree();
	void SelectMeritialStatus();

	std::wstring GenerateFileName();
};
