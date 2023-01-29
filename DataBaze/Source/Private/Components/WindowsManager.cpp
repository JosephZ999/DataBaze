#include "WindowsManager.h"
#include "DBWindowViewer.h"
#include "DBWindowWriter.h"

DBWindowsManager::DBWindowsManager(HINSTANCE HInstance)
	: HIns(HInstance)
{
	// Viewer
	const wchar_t V_CLASS_NAME[] = L"ViewerClass";

	WNDCLASS Viewer_wc = {};

	Viewer_wc.lpfnWndProc	= WndViewerProc;
	Viewer_wc.hInstance		= HInstance;
	Viewer_wc.lpszClassName = V_CLASS_NAME;
	Viewer_wc.style			= CS_GLOBALCLASS;

	RegisterClass(&Viewer_wc);

	WindowDataViewer = CreateWindowEx(0,		 // Optional window styles.
		V_CLASS_NAME,							 // Window class
		L"Viewer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style

		// Size and position
		25, 25, 600, 400, // CW_USEDEFAULT

		NULL,	   // Parent window
		NULL,	   // Menu
		HInstance, // Instance handle
		NULL	   // Additional application data
	);

	// Writer
	const wchar_t W_CLASS_NAME[] = L"WriterClass";

	WNDCLASS Writer_wc = {};

	Writer_wc.lpfnWndProc	= WndWriterProc;
	Writer_wc.hInstance		= HInstance;
	Writer_wc.lpszClassName = W_CLASS_NAME;
	Writer_wc.style			= CS_GLOBALCLASS;

	RegisterClass(&Writer_wc);

	WindowDataWriter = CreateWindowEx(0,		 // Optional window styles.
		W_CLASS_NAME,							 // Window class
		L"Writer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style

		// Size and position
		25, 25, 600, 400,

		NULL,	   // Parent window
		NULL,	   // Menu
		HInstance, // Instance handle
		NULL	   // Additional application data
	);
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
