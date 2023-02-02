#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>
#include "DBKeyCodes.h"
#include "WindowsManager.h"

DBWindowViewer* ViewerObj = nullptr;

LRESULT CALLBACK WndViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		break;
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (ViewerObj) return -1; // something is wrong

			RegisterHotKey(hWnd, HKV_Command_1, 0, VK_1);
			RegisterHotKey(hWnd, HKV_Command_2, 0, VK_2);
			RegisterHotKey(hWnd, HKV_Command_3, 0, VK_3);
			RegisterHotKey(hWnd, HKV_Command_4, 0, VK_4);
			ViewerObj = new DBWindowViewer(hWnd);
		}
		break;
	}
	case WM_CLOSE:
	{
		UnregisterHotKey(hWnd, HKV_Command_1);
		UnregisterHotKey(hWnd, HKV_Command_2);
		UnregisterHotKey(hWnd, HKV_Command_3);
		UnregisterHotKey(hWnd, HKV_Command_4);

		DBLib::SetWindowVisibility(hWnd, false);

		if (ViewerObj)
		{
			delete ViewerObj;
			ViewerObj = nullptr;
		}

		return 0;
	}

	case WM_HOTKEY:
	{
		if (! ViewerObj) break;

		switch (wParam)
		{
		case 1: ViewerObj->Autofill_Form1(); break;
		case 2: ViewerObj->Autofill_Form2(); break;
		case 3: ViewerObj->Autofill_Form3(); break;
		case 4: ViewerObj->Autofill_Form4(); break;
		}
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowViewer::DBWindowViewer(DBInterface* InOwner)
{
	SetOwner(InOwner);
	auto Manager = static_cast<DBWindowsManager*>(GetOwner());
	if (Manager)
	{
		WindowHandle = Manager->GetViewerHandle();
	}
}

void DBWindowViewer::Autofill_Form1()
{
	// Sleep(1000);
	DBLib::CopyToClipboard(WindowHandle, "123456789");
	DBLib::PasteClipboard();
}

void DBWindowViewer::Autofill_Form2() {}

void DBWindowViewer::Autofill_Form3() {}

void DBWindowViewer::Autofill_Form4() {}
