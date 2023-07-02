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
		if (ManagerRef->GetViewer())
		{
			return ManagerRef->GetViewer()->CallProc(hWnd, message, wParam, lParam);
		}
	}
	else if (hWnd == ManagerRef->GetWriterHandle())
	{
		if (ManagerRef->GetWriter())
		{
			return ManagerRef->GetWriter()->CallProc(hWnd, message, wParam, lParam);
		}
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
		25, 25, 600, 460,						 // CW_USEDEFAULT
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

	const Size2D WriterSize		= {600, 365};
	const Size2D WriterHalf		= WriterSize / 2;
	const Size2D ScreenSize		= DBLib::GetScreenSize();
	const Size2D ScreenCenter	= ScreenSize / 2;
	const Size2D FinalWriterPos = ScreenCenter - WriterHalf;

	WriterHandle = CreateWindowEx(0,			 // Optional window styles. (Transparent bg WS_EX_TRANSPARENT)
		W_CLASS_NAME,							 // Window class
		L"Writer",								 // Window text
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION, // Window style (Transparent bg WS_POPUP)
		FinalWriterPos.X, FinalWriterPos.Y,		 // position
		WriterSize.X, WriterSize.Y,				 // Size
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

void DBWindowsManager::OpenWindowByType(EWindows WindowType)
{
	switch (WindowType)
	{
	case EWindows::IDW_VIEWER:
	{
		CreateViewer();
		SendMessage(ViewerHandle, WM_COMMAND, WM_SHOWWINDOW, 0);
		if (WindowViewer)
		{
			WindowViewer->OnClose.Bind(this, &DBWindowsManager::OnViewerCloseHandle);
		}
		return;
	}
	case EWindows::IDW_WRITER:
	{
		CreateWriter();
		SendMessage(WriterHandle, WM_COMMAND, WM_SHOWWINDOW, 0);
		if (WindowWriter)
		{
			WindowWriter->OnClose.Bind(this, &DBWindowsManager::OnWriterCloseHandle);
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
		if (WindowViewer)
		{
			SendMessage(ViewerHandle, WM_SHOWWINDOW, FALSE, 0);
			DestroyViewer();
			ShowWindow(ViewerHandle, SW_HIDE);
		}
		break;
	}
	case EWindows::IDW_WRITER:
	{
		if (WindowWriter)
		{
			SendMessage(WriterHandle, WM_SHOWWINDOW, FALSE, 0);
			DestroyWriter();
			ShowWindow(WriterHandle, SW_HIDE);
		}
		break;
	}
	} // switch end
}

void DBWindowsManager::OnWriterCloseHandle()
{
	CloseWindowByType(IDW_WRITER);
}

void DBWindowsManager::OnViewerCloseHandle()
{
	CloseWindowByType(IDW_VIEWER);
}

void DBWindowsManager::SetViewerData(FMemberId InId, const DBFamilyData& InData)
{
	if (! GetViewer()) return;
	GetViewer()->SetMemberData(InId, InData);
}

void DBWindowsManager::SetWriterData(FMemberId InId, const DBFamilyData& Data, EPeopleType People)
{
	if (! GetWriter()) return;
	GetWriter()->EditPeople(InId, Data, People);
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
	if (! WindowViewer)
	{
		WindowViewer = new DBWindowViewer(GetViewerHandle());
	}
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
	if (! WindowWriter)
	{
		WindowWriter = new DBWindowWriter(GetWriterHandle());
	}
}

void DBWindowsManager::DestroyWriter()
{
	if (WindowWriter)
	{
		delete WindowWriter;
		WindowWriter = nullptr;
	}
}
