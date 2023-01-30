
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"

DBWindowWriter* WriterObj = nullptr;
DBWindow		EditBox;

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		EditBox.Id		  = EDBButtonId::IDB_MAX;
		EditBox.Parent	  = hWnd;
		EditBox.Position = {25, 150};
		EditBox.Size	  = {520, 40};
		EditBox.Text	  = {};

		// DBLib::CreateButton(NewBtn);
		DBLib::CreateEditBox(EditBox, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE);
		
		DBLib::SetFontSize(EditBox.Window, 32);
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (WriterObj) return -1; // something is wrong

			WriterObj = new DBWindowWriter;
			SetFocus(EditBox.Window);
		}
		break;
	}
	case WM_PAINT:
	{
		DBLib::CreateText(hWnd);
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_RETURN)
		{
			// MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
		break;
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		if (WriterObj)
		{
			delete WriterObj;
			WriterObj = nullptr;
		}
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
