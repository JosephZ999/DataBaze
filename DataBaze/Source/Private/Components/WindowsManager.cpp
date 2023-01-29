#include "WindowsManager.h"

DBWindowsManager::DBWindowsManager(HINSTANCE* HInstance)
	: HIns(HInstance)
{
}

void DBWindowsManager::CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam) {}

void DBWindowsManager::OpenWindow(EWindows WindowType)
{
	switch (WindowType)
	{
	case EWindows::IDW_VIEWER:
	{
		ShowWindow(WindowDataViewer, SW_SHOWDEFAULT);
		ShowWindow(WindowDataWriter, SW_HIDE);
		return;
	}
	case EWindows::IDW_WRITER:
	{
		ShowWindow(WindowDataWriter, SW_SHOWDEFAULT);
		ShowWindow(WindowDataViewer, SW_HIDE);
		return;
	}
	} // switch end
}

void DBWindowsManager::CloseWindow(EWindows WindowType)
{
	switch (WindowType)
	{
	case EWindows::IDW_VIEWER:
	{
		ShowWindow(WindowDataViewer, SW_HIDE);
		return;
	}
	case EWindows::IDW_WRITER:
	{
		ShowWindow(WindowDataWriter, SW_HIDE);
		return;
	}
	} // switch end
}
