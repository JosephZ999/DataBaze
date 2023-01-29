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
		NewBtn.Size		= {100, 50};

		DBLib::CreateButton(NewBtn);
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}
