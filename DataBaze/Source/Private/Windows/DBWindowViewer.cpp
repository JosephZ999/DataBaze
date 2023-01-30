#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>

DBWindowViwer* ViewerObj = nullptr;

LRESULT CALLBACK WndViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		RegisterHotKey(hWnd, 1, MOD_CONTROL | MOD_SHIFT, VK_RETURN);
		RegisterHotKey(hWnd, 2, 0, VK_SPACE);
		break;
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (ViewerObj) return -1; // something is wrong

			ViewerObj = new DBWindowViwer;
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
		if (wParam == 2) // hot key id
		{
			if (ViewerObj)
			{
				Sleep(1000);
				DBLib::CopyToClipboard(hWnd, "123456789");
				DBLib::PasteClipboard();
			}
		}
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
