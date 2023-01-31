#pragma once
#include "framework.h"
#include "resource.h"

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
};
