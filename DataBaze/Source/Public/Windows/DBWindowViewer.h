#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"
#include "DBDelegate.h"

class DBButtonManager;

enum EHotKeyId
{
	HK_None = 0,

	// Viewer
	HKV_Command_1,
	HKV_Command_2,
	HKV_Command_3,
	HKV_Command_4,
	HKV_Next,

	// Writer
	HKW_Enter,
};

enum EAutoFillStep
{
	AFS_None,
	AFS_Part1,
	AFS_Part2,
	AFS_Check,
	AFS_CopyResult,
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
	DBButtonManager* ButtonManager;
	bool			 HasSpouse = false;
	bool			 HasChild  = false;
	std::wstring	 Title;
	std::string		 Info;
	int				 SelectedMemberId = 0;
	int				 SelectedFolderId = 1;

	EPeopleType CurrentPeople = PT_Parent;

	std::vector<EAutoFillStep> StepMap;
	EAutoFillStep CurrentStep = EAutoFillStep::AFS_None;

public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void AutoFill();
	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();

	void SetMemberData(int MemberId, int FolderId, const DBFamilyData& InData);

private:
	void InitializeSteps();
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
	
	std::wstring GenerateFileName();
};
