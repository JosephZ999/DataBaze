
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"

DBWindowWriter* WriterObj = nullptr;
DBWindow		WriterEditBox;
DBWindow		WriterInfoBox;

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		WriterEditBox.Id	   = EDBWinCompId::IDC_W_Edit;
		WriterEditBox.Parent   = hWnd;
		WriterEditBox.Position = {25, 150};
		WriterEditBox.Size	   = {520, 40};
		WriterEditBox.Text	   = {};
		WriterEditBox.FontSize = 32;

		DBLib::CreateEditBox(WriterEditBox, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE);

		WriterInfoBox.Id	   = EDBWinCompId::IDC_W_Info;
		WriterInfoBox.Parent   = hWnd;
		WriterInfoBox.Position = {25, 25};
		WriterInfoBox.Size	   = {520, 120};
		WriterInfoBox.Text	   = L"Info Info Info Info Info Info Info Info Info Info Info Info Info Info";
		WriterInfoBox.FontSize = 24;

		DBLib::CreateStaticBox(WriterInfoBox, WS_VISIBLE | WS_CHILD);
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (WriterObj) return -1; // something is wrong

			WriterObj = new DBWindowWriter(hWnd);
			SetFocus(WriterEditBox.Window);
		}
		break;
	}
	case WM_PAINT:
	{
		// DBLib::CreateText(hWnd);
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
