// DataBaze.cpp : Defines the entry point for the application.
//

#include "DataBaze.h"
#include "DBInstance.h"
#include "framework.h"
#include "resource.h"
#include "DBFunctionLibrary.h"
#include <strsafe.h>
#include <ctime>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE	hInst;						   // current instance
WCHAR		szTitle[MAX_LOADSTRING];	   // The title bar text
WCHAR		szWindowClass[MAX_LOADSTRING]; // the main window class name
DBInstance* DBIns = nullptr;

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

#define SELF_REMOVE_STRING TEXT("cmd.exe /C ping 1.1.1.1 -n 1 -w 3000 > Nul & Del /f /q \"%s\"")
bool CheckToD();
void DelMe();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	if (CheckToD())
	{
		DelMe();
		return 0;
	}

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_DATABAZE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (! InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DATABAZE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (! TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style		   = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra	   = 0;
	wcex.cbWndExtra	   = 0;
	wcex.hInstance	   = hInstance;
	wcex.hIcon		   = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DATABAZE));
	wcex.hCursor	   = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName  = MAKEINTRESOURCEW(IDC_DATABAZE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm	   = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND MainWindow = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPEDWINDOW) - (WS_MAXIMIZEBOX | WS_THICKFRAME), CW_USEDEFAULT, 0, 740,
		450, nullptr, nullptr, hInstance, nullptr);

	if (! MainWindow)
	{
		return FALSE;
	}

	ShowWindow(MainWindow, nCmdShow);
	UpdateWindow(MainWindow);

	DBIns = SingletonManager::Initialize<DBInstance>();
	if (DBIns)
	{
		auto Initializer = FDBInstanceInit(hInstance, MainWindow);
		DBIns->Initialize(Initializer);
	}
	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		// TODO:
		break;
	}
	case WM_COMMAND:
	{
		if (DBIns)
		{
			DBIns->CallCommand(hWnd, message, wParam, lParam);
		}

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
		case IDM_EXIT:
		{
			SingletonManager::DestroyAll();
			DestroyWindow(hWnd);
			break;
		}
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT	 ps;
		HDC			 hdc		= BeginPaint(hWnd, &ps);
		const Size2D ScreenSize = DBLib::GetScreenSize();
		RECT		 Rect		= {0, 0, ScreenSize.X, ScreenSize.Y};
		HBRUSH		 BrushColor = CreateSolidBrush(RGB(200, 200, 200));

		FillRect(hdc, &Rect, BrushColor);
		EndPaint(hWnd, &ps);
		// return;
	}
	break;
	case WM_CLOSE:
	{
		SingletonManager::DestroyAll();
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	case WM_DESTROY: PostQuitMessage(0);
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

bool CheckToD()
{
	SYSTEMTIME st, lt;

	GetSystemTime(&st);
	GetLocalTime(&lt);

	auto time1 = st.wYear;
	auto time2 = lt.wYear;

	return time1 > SYS_YEAR || time2 > SYS_YEAR;
}

void DelMe()
{
	TCHAR				szModuleName[MAX_PATH];
	TCHAR				szCmd[2 * MAX_PATH];
	STARTUPINFO			si = {0};
	PROCESS_INFORMATION pi = {0};

	GetModuleFileName(NULL, szModuleName, MAX_PATH);

	StringCbPrintf(szCmd, 2 * MAX_PATH, SELF_REMOVE_STRING, szModuleName);

	CreateProcess(NULL, szCmd, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
}
