// DataBaze.cpp : Defines the entry point for the application.
//

#include "DataBaze.h"

// Global Variables:
HINSTANCE hInst;						 // current instance
WCHAR	  szTitle[MAX_LOADSTRING];		 // The title bar text
WCHAR	  szWindowClass[MAX_LOADSTRING]; // the main window class name
HWND	  MainWindow;
HWND	  ListBox;
DBSystem* System;
Size2D	  BtnSize = {150, 30};

// Forward declarations of functions included in this code module:
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

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

	MainWindow = CreateWindowW(
		szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (! MainWindow)
	{
		return FALSE;
	}

	ShowWindow(MainWindow, nCmdShow);
	UpdateWindow(MainWindow);

	System = new DBSystem(&hInst, MainWindow);
	if (System)
	{
		System->BtnSize = &BtnSize;

		// Viewer
		{
			const wchar_t CLASS_NAME[] = L"ViewerClass";

			WNDCLASS wc = {};

			wc.lpfnWndProc	 = WndProc;
			wc.hInstance	 = hInstance;
			wc.lpszClassName = CLASS_NAME;
			wc.style		 = CS_GLOBALCLASS;

			RegisterClass(&wc);

			auto Viewer = CreateWindowEx(0, // Optional window styles.
				CLASS_NAME,					// Window class
				L"Viewer",					// Window text
				WS_OVERLAPPED,				// Window style

				// Size and position
				25, 25, 600, 400, // CW_USEDEFAULT

				NULL,	   // Parent window
				NULL,	   // Menu
				hInstance, // Instance handle
				NULL	   // Additional application data
			);

			if (Viewer)
			{
				System->SetWindowsViewer(Viewer);
			}
		}

		// Writer
		{
			const wchar_t CLASS_NAME[] = L"WriterClass";

			WNDCLASS wc = {};

			wc.lpfnWndProc	 = WndProc;
			wc.hInstance	 = hInstance;
			wc.lpszClassName = CLASS_NAME;
			wc.style		 = CS_GLOBALCLASS;

			RegisterClass(&wc);

			auto Writer = CreateWindowEx(0, // Optional window styles.
				CLASS_NAME,					// Window class
				L"Writer",					// Window text
				WS_OVERLAPPED,				// Window style

				// Size and position
				25, 25, 600, 400,

				NULL,	   // Parent window
				NULL,	   // Menu
				hInstance, // Instance handle
				NULL	   // Additional application data
			);

			if (Writer)
			{
				System->SetWindowsWriter(Writer);
			}
		}

		System->EndConstruct();
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
		if (System)
		{
			System->CallCommand(hWnd, message, wParam, lParam);
		}

		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT: DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About); break;
		case IDM_EXIT: DestroyWindow(hWnd); break;
		default: return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hWnd, &ps);

		// TODO: Add any drawing code that uses hdc here...
		// TCHAR greeting[] = _T("Hello, Windows desktop!");
		// TextOut(hdc, 5, 5, greeting, _tcslen(greeting));

		if (System)
		{
			System->CallPaint(hWnd, message, wParam, lParam);
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY: PostQuitMessage(0); break;
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
