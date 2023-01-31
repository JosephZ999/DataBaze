#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>
#include "DBKeyCodes.h"

DBWindowViwer* ViewerObj = nullptr;

LRESULT CALLBACK WndViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RegisterHotKey(hWnd, 1, 0, VK_1);
		RegisterHotKey(hWnd, 2, 0, VK_2);
		RegisterHotKey(hWnd, 3, 0, VK_3);
		RegisterHotKey(hWnd, 4, 0, VK_4);
		break;
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (ViewerObj) return -1; // something is wrong

			ViewerObj = new DBWindowViwer(hWnd);
		}
		break;
	}
	case WM_CLOSE:
	{
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

void DBWindowViwer::Autofill_Form1()
{
	// Sleep(1000);
	DBLib::CopyToClipboard(OwnerHWND, "123456789");
	DBLib::PasteClipboard();
}

void DBWindowViwer::Autofill_Form2() {}

void DBWindowViwer::Autofill_Form3() {}

void DBWindowViwer::Autofill_Form4() {}
