
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"

DBWindowWriter* WriterObj = nullptr;

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (WriterObj) return -1; // something is wrong

			WriterObj = new DBWindowWriter;
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
