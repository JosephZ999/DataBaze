#pragma once

#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

#include "DBInterface.h"

LRESULT CALLBACK WndViewerProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowViewer : public DBInterface
{
public:
	DBWindowViewer(DBInterface* InOwner);
	HWND		 WindowHandle = 0;
	DBFamilyData MemberData;

public:
	void SetMemberData(const DBFamilyData& InData);
	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();

public:
	void OnConstruct();
};
