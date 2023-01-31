#pragma once

#include "framework.h"
#include "resource.h"

LRESULT CALLBACK WndViewerProc(HWND, UINT, WPARAM, LPARAM);

class DBWindowViwer
{
	DBWindowViwer() {}

public:
	DBWindowViwer(HWND InOwnerHWND)
		: OwnerHWND(InOwnerHWND)
	{
	}

	HWND OwnerHWND = 0;

public:
	void Autofill_Form1();
	void Autofill_Form2();
	void Autofill_Form3();
	void Autofill_Form4();
};
