#include "DbWindowsManager.h"
#include "DBWindowViewer.h"
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"

DBWindowsManager* ManagerRef = nullptr;

LRESULT CALLBACK WndManagerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (! ManagerRef) return 0;

	if (hWnd == ManagerRef->GetViewerHandle())
	{
		return ManagerRef->GetViewer()->CallProc(hWnd, message, wParam, lParam);
	}
	else if (hWnd == ManagerRef->GetWriterHandle())
	{
		return ManagerRef->GetWriter()->CallProc(hWnd, message, wParam, lParam);
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void DBWindowsManager::Initialize(HINSTANCE HInstance)
{
	ManagerRef = this;

	// Viewer
	const wchar_t V_CLASS_NAME[] = L"ViewerClass";

	WNDCLASS Viewer_wc = {};

	Viewer_wc.lpfnWndProc	= WndManagerProc;
	Viewer_wc.hInstance		= HInstance;
	Viewer_wc.lpszClassName = V_CLASS_NAME;
	Viewer_wc.style			= CS_GLOBALCLASS;

	RegisterClass(&Viewer_wc);

	ViewerHandle = CreateWindowEx(0,			 // Optional window styles. (Transparent bg WS_EX_TRANSPARENT)
		V_CLASS_NAME,							 // Window class
		L"Viewer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style (Transparent bg WS_POPUP)
		25, 25, 600, 430,						 // CW_USEDEFAULT
		NULL,									 // Parent window
		NULL,									 // Menu
		HInstance,								 // Instance handle
		NULL									 // Additional application data
	);

	// Writer
	const wchar_t W_CLASS_NAME[] = L"WriterClass";

	WNDCLASS Writer_wc = {};

	Writer_wc.lpfnWndProc	= WndManagerProc;
	Writer_wc.hInstance		= HInstance;
	Writer_wc.lpszClassName = W_CLASS_NAME;
	Writer_wc.style			= CS_GLOBALCLASS;

	RegisterClass(&Writer_wc);

	const Size2D WriterSize		= {600, 400};
	const Size2D WriterHalf		= WriterSize / 2;
	const Size2D ScreenSize		= DBLib::GetScreenSize();
	const Size2D ScreenCenter	= ScreenSize / 2;
	const Size2D FinalWriterPos = ScreenCenter - WriterHalf;

	WriterHandle = CreateWindowEx(0,				  // Optional window styles. (Transparent bg WS_EX_TRANSPARENT)
		W_CLASS_NAME,								  // Window class
		L"Writer",									  // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION,	  // Window style (Transparent bg WS_POPUP)
		FinalWriterPos.X, FinalWriterPos.Y, 600, 400, // Size and position
		NULL,										  // Parent window
		NULL,										  // Menu
		HInstance,									  // Instance handle
		NULL										  // Additional application data
	);

	//GetViewer()->CallProc(GetViewerHandle(), WM_CREATE, 0, 0);
	//GetWriter()->CallProc(GetWriterHandle(), WM_CREATE, 0, 0);
}

DBWindowsManager::~DBWindowsManager()
{
	DestroyViewer();
	DestroyWriter();
}

void DBWindowsManager::OpenWindowByType(EWindows WindowType)
{
	switch (WindowType)
	{
	case EWindows::IDW_VIEWER:
	{
		CreateViewer();
		DestroyWriter();
		ShowWindow(ViewerHandle, SW_SHOWDEFAULT);
		CloseWindowByType(EWindows::IDW_WRITER);
		SendMessage(ViewerHandle, WM_COMMAND, WM_SHOWWINDOW, 0);
		if (WindowViewer)
		{
		}
		return;
	}
	case EWindows::IDW_WRITER:
	{
		CreateWriter();
		DestroyViewer();
		ShowWindow(WriterHandle, SW_SHOWDEFAULT);
		CloseWindowByType(EWindows::IDW_VIEWER);
		SendMessage(WriterHandle, WM_COMMAND, WM_SHOWWINDOW, 0);
		if (WindowWriter)
		{
			WindowWriter->OnWriteSuccess.Bind(this, &DBWindowsManager::OnMemberAddedHandle);
		}
		return;
	}
	} // switch end
}

void DBWindowsManager::CloseWindowByType(EWindows WindowType)
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

void DBWindowsManager::OnMemberAddedHandle()
{
	CloseWindowByType(EWindows::IDW_WRITER);
}

void DBWindowsManager::SetViewerData(const DBFamilyData& InData)
{
	GetViewer()->SetMemberData(InData);
}

DBInterface* DBWindowsManager::GetSystem() const
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
