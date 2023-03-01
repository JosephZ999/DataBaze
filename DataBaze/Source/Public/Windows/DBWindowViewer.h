#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"

class DBButtonManager;

struct FillData
{
	std::wstring Title;
	std::string	 Info;
};

class DBWindowViewer : public DBInterface
{
public:
	DBWindowViewer(DBInterface* InOwner);
	HWND		 WindowHandle = 0;
	DBFamilyData MemberData;

private:
	DBButtonManager* ButtonManager;
	bool		 HasSpouse = false;
	bool		 HasChild  = false;
	std::wstring Title;
	std::string	 Info;

public:
	LRESULT CALLBACK CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();

	void SetMemberData(const DBFamilyData& InData);

private:
	void PrintData();
	void PrintPeople(const DBPeopleData& People, bool IsChild);
	void PrintMail();

	inline void PasteString(const std::string Text);
};
