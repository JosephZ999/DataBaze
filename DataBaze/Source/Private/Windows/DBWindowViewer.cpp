#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>
#include "DBKeyCodes.h"
#include "DBWindowsManager.h"

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
			RegisterHotKey(hWnd, HKV_Command_1, 0, VK_1);
			RegisterHotKey(hWnd, HKV_Command_2, 0, VK_2);
			RegisterHotKey(hWnd, HKV_Command_3, 0, VK_3);
			RegisterHotKey(hWnd, HKV_Command_4, 0, VK_4);
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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hWnd, &ps);

		Size2D WindowSize = DBLib::GetScreenSize(hWnd);
		RECT   Rect		  = {0, 0, WindowSize.X, WindowSize.Y};
		HBRUSH BrushColor = CreateSolidBrush(RGB(200, 220, 200));
		FillRect(hdc, &Rect, BrushColor);

		EndPaint(hWnd, &ps);
		break;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowViewer::DBWindowViewer(DBInterface* InOwner)
{
	SetOwner(InOwner);
	// auto Manager = static_cast<DBWindowsManager*>(GetOwner());
	auto Manager = Cast<DBWindowsManager>(GetOwner());
	if (Manager)
	{
		WindowHandle = Manager->GetViewerHandle();
		ViewerObj	 = this;
	}
}

void DBWindowViewer::OnConstruct()
{
	std::wstring message;
	DBConvert::StringToWString(MemberData.Parents[0].Name, message);
	MessageBox(NULL, message.c_str(), L"Dialog Box", MB_OK);
}

void DBWindowViewer::SetMemberData(const DBFamilyData& InData)
{
	MemberData = InData;
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
