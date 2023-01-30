#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"

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
		RegisterHotKey(hWnd, 2, MOD_CONTROL | MOD_SHIFT, VK_BACK);
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
			//SendInput()
			MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
