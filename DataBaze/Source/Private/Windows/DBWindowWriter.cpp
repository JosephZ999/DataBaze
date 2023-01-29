#include "framework.h"
#include "resource.h"
#include "DBFunctionLibrary.h"

LRESULT CALLBACK WndWriterProc(HWND, UINT, WPARAM, LPARAM);

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}