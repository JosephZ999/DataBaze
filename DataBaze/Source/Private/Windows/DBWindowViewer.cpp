#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>

LRESULT CALLBACK WndViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		DBButton NewBtn;
		NewBtn.Id		= EDBButtonId::IDB_MAX;
		NewBtn.Parent	= hWnd;
		NewBtn.Position = {0, 0};
		NewBtn.Size		= {200, 100};

		// DBLib::CreateButton(NewBtn);
		DBLib::CreateComboBox(NewBtn);

		RegisterHotKey(hWnd, 1, MOD_CONTROL | MOD_SHIFT, VK_RETURN);
		RegisterHotKey(hWnd, 2, 0, VK_SPACE);
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}
	case WM_COMMAND:
	{

		break;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			// MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
		break;
	}
	case WM_HOTKEY:
	{
		if (wParam == 2) // hot key id
		{

			// Sleep(3000);

			// INPUT ip;
			// ip.type			  = INPUT_KEYBOARD;
			// ip.ki.wScan		  = 0;
			// ip.ki.time		  = 0;
			// ip.ki.dwExtraInfo = 0;

			// ip.ki.wVk	  = VK_SHIFT;
			// ip.ki.dwFlags = 0;
			// SendInput(1, &ip, sizeof(INPUT));

			// ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
			// SendInput(1, &ip, sizeof(INPUT));

			// ip.ki.wVk	  = VK_F5;
			// ip.ki.dwFlags = 0; // KEYEVENTF_KEYUP for key release
			// SendInput(1, &ip, sizeof(INPUT));

			// ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
			// SendInput(1, &ip, sizeof(INPUT));

			std::string NewString("AOAOAOAAO");
			DBLib::ToClipboard(hWnd, NewString);

			INPUT ip;
			ip.type			  = INPUT_KEYBOARD;
			ip.ki.wScan		  = 0;
			ip.ki.time		  = 0;
			ip.ki.dwExtraInfo = 0;

			// Press Control
			ip.ki.wVk	  = VK_CONTROL;
			ip.ki.dwFlags = 0;
			SendInput(1, &ip, sizeof(INPUT));

			// Press V
			ip.ki.wVk = 0x56;
			SendInput(1, &ip, sizeof(INPUT));

			// Release V
			ip.ki.dwFlags = KEYEVENTF_KEYUP;
			SendInput(1, &ip, sizeof(INPUT));

			// Release Control
			ip.ki.wVk = VK_CONTROL;
			SendInput(1, &ip, sizeof(INPUT));

			MessageBox(NULL, L"Success", L"Dialog Box", MB_OK);
		}
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
