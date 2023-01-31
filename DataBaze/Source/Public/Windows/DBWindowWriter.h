#pragma once
#include "framework.h"
#include "resource.h"
#include "DBDataTypes.h"

LRESULT CALLBACK WndWriterProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowWriter
{
	DBWindowWriter() {}

public:
	DBWindowWriter(HWND InOwnerHWND)
		: OwnerHWND(InOwnerHWND)
	{
	}

	HWND OwnerHWND = 0;

	DBFamilyData MembersData;
};
