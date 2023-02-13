#include "DbWindowsManager.h"
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

	ViewerHandle = CreateWindowEx(0,			 // Optional window styles.
		V_CLASS_NAME,							 // Window class
		L"Viewer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style
		25, 25, 600, 400,						 // CW_USEDEFAULT
		NULL,									 // Parent window
		NULL,									 // Menu
		HInstance,								 // Instance handle
		NULL									 // Additional application data
	);

	// Writer
	const wchar_t W_CLASS_NAME[] = L"WriterClass";

	WNDCLASS Writer_wc = {};

	Writer_wc.lpfnWndProc	= WndWriterProc;
	Writer_wc.hInstance		= HInstance;
	Writer_wc.lpszClassName = W_CLASS_NAME;
	Writer_wc.style			= CS_GLOBALCLASS;

	RegisterClass(&Writer_wc);

	WriterHandle = CreateWindowEx(0,			 // Optional window styles.
		W_CLASS_NAME,							 // Window class
		L"Writer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style
		25, 25, 600, 400,						 // Size and position
		NULL,									 // Parent window
		NULL,									 // Menu
		HInstance,								 // Instance handle
		NULL									 // Additional application data
	);
}

DBWindowsManager::~DBWindowsManager()
{
	DestroyViewer();
	DestroyWriter();
}

void DBWindowsManager::OpenWindow(EWindows WindowType)
{
	switch (WindowType)
	{
	case EWindows::IDW_VIEWER:
	{
		CreateViewer();
		DestroyWriter();
		ShowWindow(ViewerHandle, SW_SHOWDEFAULT);
		ShowWindow(WriterHandle, SW_HIDE);
		SendMessage(ViewerHandle, WM_COMMAND, WM_SHOWWINDOW, 0);

		if (WindowViewer)
		{
			WindowViewer->OnConstruct();
		}
		return;
	}
	case EWindows::IDW_WRITER:
	{
		CreateWriter();
		DestroyViewer();
		ShowWindow(WriterHandle, SW_SHOWDEFAULT);
		ShowWindow(ViewerHandle, SW_HIDE);
		SendMessage(WriterHandle, WM_COMMAND, WM_SHOWWINDOW, 0);

		if (WindowWriter)
		{
			WindowWriter->OnConstruct();
		}
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
		DestroyViewer();
		ShowWindow(ViewerHandle, SW_HIDE);
		return;
	}
	case EWindows::IDW_WRITER:
	{
		DestroyWriter();
		ShowWindow(WriterHandle, SW_HIDE);
		return;
	}
	} // switch end
}

DBInterface* DBWindowsManager::GetSystem()
{
	return GetOwner();
}

DBInterface* DBWindowsManager::GetViewerClass() const
{
	return WindowViewer;
}

DBInterface* DBWindowsManager::GetWriterClass() const
{
	return WindowWriter;
}

void DBWindowsManager::CreateViewer()
{
	if (WindowViewer)
	{
		delete WindowViewer;
		WindowViewer = nullptr;
	}
	WindowViewer = new DBWindowViewer(this);
}

void DBWindowsManager::DestroyViewer()
{
	if (WindowViewer)
	{
		delete WindowViewer;
		WindowViewer = nullptr;
	}
}

void DBWindowsManager::CreateWriter()
{
	if (WindowWriter)
	{
		delete WindowWriter;
		WindowWriter = nullptr;
	}
	WindowWriter = new DBWindowWriter(this);
}

void DBWindowsManager::DestroyWriter()
{
	if (WindowWriter)
	{
		delete WindowWriter;
		WindowWriter = nullptr;
	}
}
