#pragma once

#include "framework.h"
#include "Resource.h"
#include "DBDataTypes.h"

class DBWindowsManager
{

public:
	DBWindowsManager(HINSTANCE* HInstance);
	HINSTANCE* HIns;

protected:
	HWND WindowDataViewer = 0;
	HWND WindowDataWriter = 0;
	void CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam);

public:
	void OpenWindow(EWindows WindowType);
	void CloseWindow(EWindows WindowType);


	friend class DBSystem;
};
