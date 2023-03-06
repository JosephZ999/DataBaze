#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"
#include "DBDelegate.h"

class DBButtonManager;

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

public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();

	void SetMemberData(int MemberId, int FolderId, const DBFamilyData& InData);

private:
	void ChangePeople(bool Next);

	void PrintData();
	void PrintPeople(const DBPeopleData& People, bool IsChild);
	void PrintMail();
	void PressTab(size_t Times = 1);

	inline void PasteString(const std::string Text);
	inline void WriteEMail();
	void WriteString(std::string Text);
};
