#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"

LRESULT CALLBACK WndViewerProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowViewer : public DBInterface
{

	DBFamilyData MemberData;

public:
	DBWindowViewer() {}
	DBWindowViewer(DBInterface* InOwner);
	DBWindowViewer(HWND InOwnerHWND)
		: WindowHandle(InOwnerHWND)
	{
	}

	HWND WindowHandle = 0;

public:
	void OnConstruct();

	void SetMemberData(const DBFamilyData& InData);
	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();
};
