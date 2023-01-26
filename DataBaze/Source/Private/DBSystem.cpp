#include "Source/Public/DBSystem.h"

DBSystem::DBSystem(HINSTANCE* HInstance)
{
	HIns = HInstance;

	struct LocalBtnInfo
	{
		DBButtonId	 Id;
		Size2D		 Pos;
		std::wstring Text;
	};

	const LocalBtnInfo MainButtons[] = {
		//
		{DBButtonId::IDB_DIALOG, Size2D(25, 25), L"Show List"},		//
	{DBButtonId::IDB_NEWITEM, Size2D(25, 75), L"Add New Item"}, //
	{DBButtonId::IDB_LOCK, Size2D(25, 125), L"Lock"},			//
	{DBButtonId::IDB_UNLOCK, Size2D(25, 175), L"Unlock"}		//
	};

	for (auto BtnInfo : MainButtons)
	{
		CreateButton(MainWindow, BtnInfo.Text, BtnInfo.Id, BtnInfo.Pos, BtnSize);
	}

	InitListBox();
}

DBSystem::~DBSystem() {}

void DBSystem::CreateButton(const HWND& ParentWindow, const std::wstring Text, DBButtonId Id, Size2D Pos, Size2D Size)
{
	auto NewWindow = CreateWindow(L"button", Text.c_str(), //
		WS_VISIBLE | WS_CHILD,							   //
		Pos.X, Pos.Y, Size.X, Size.Y,					   //
		ParentWindow, (HMENU)Id, NULL, NULL);

	SetFontSize(NewWindow, 16);

	Buttons.Add(DBButtonBase(Id, NewWindow, Pos, Size, Text));
}

void DBSystem::InitListBox()
{
	// Init ListContainer

	ListContainer.Items.push_back(DBListItem({DBPeopleData(L"Abdu")}, {}, false));
	ListContainer.Items.push_back(DBListItem({DBPeopleData(L"Amir")}, {}, false));
	ListContainer.Items.push_back(DBListItem({DBPeopleData(L"Piyoz")}, {}, false));
	ListContainer.Items.push_back(DBListItem({DBPeopleData(L"Hechkim")}, {}, false));
	ListContainer.Items.push_back(DBListItem({DBPeopleData(L"Komp")}, {}, false));

	ListBox = CreateWindow(L"LISTBOX", L"button", WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD | LBS_STANDARD, 200, 25, 500, 300, MainWindow,
		(HMENU)IDC_LISTBOX, NULL, NULL);

	for (auto Family : ListContainer.Items)
	{
		if (! Family.bFamily)
		{
			auto ItemName = Family.Parents[0].Name.c_str();
			SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)ItemName);
		}
		else
		{
			for (auto Parents : Family.Parents)
			{
				SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)L"People (Family)");
			}
		}
	}
	SetFontSize(ListBox, 20);
}

void DBSystem::CallCommand(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam)
{
	switch (LOWORD(WParam))
	{
	case IDC_LISTBOX:
	{
		switch (HIWORD(WParam))
		{
		case LBN_DBLCLK:
		{
			// Get selected index.
			int lbItem = (int)SendMessage(ListBox, LB_GETCURSEL, 0, 0);

			// Get item data.
			int i = (int)SendMessage(ListBox, LB_GETITEMDATA, lbItem, 0);

			MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
		}
	}
	}

	DBButtonId BtnId = static_cast<DBButtonId>(LOWORD(WParam));
	switch (BtnId)
	{
	case IDB_DIALOG:
	{
	}
	break;
	case IDB_NEWITEM:
	{
		++clicks;
		KillTimer(NULL, TimerId);
		TimerId = SetTimer(NULL, 0, 200, &TimerProc);
		if (clicks == 2)
		{
			MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
	}
	break;
	case IDB_LOCK:
	{
		MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
	}
	break;
	case IDB_UNLOCK:
	{
		MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
	}
	break;
	}
}

void DBSystem::CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam)
{
	if (! System) return;

	RECT Rect;
	if (GetWindowRect(MainWindow, &Rect))
	{
		WindowSize.X = Rect.right - Rect.left;
		WindowSize.Y = Rect.bottom - Rect.top;
	}
	Update_BtnVisibility();
	Update_ListBoxScale();
}

void DBSystem::Update_BtnVisibility()
{
	if (Buttons.IsEmpty()) return;

	if (IsPortraitModeEnabled())
	{
		HideButton(IDB_DIALOG);
		HideButton(IDB_NEWITEM);
		HideButton(IDB_LOCK);
		HideButton(IDB_UNLOCK);
	}
	else
	{
		ShowButton(IDB_DIALOG);
		ShowButton(IDB_NEWITEM);
		ShowButton(IDB_LOCK);
		ShowButton(IDB_UNLOCK);
	}
}

void DBSystem::Update_ListBoxScale()
{
	Size2D Pos = (IsPortraitModeEnabled()) ? Size2D(25, 25) : Size2D(BtnSize.X + 50, 25);
	Size2D Size;
	Size.X = abs(Pos.X - (WindowSize.X - 25));
	Size.Y = WindowSize.Y - 100;

	SetWindowPos(ListBox, 0, //
		Pos.X, Pos.Y,		 //
		Size.X, Size.Y,		 //
		1);
}

void DBSystem::HideButton(DBButtonId Id)
{
	DBButtonBase Button;
	if (Buttons.FindByIndex(Id, Button))
	{
		SetWindowPos(Button.Window, 0,			  //
			Button.Position.X, Button.Position.Y, //
			0, 0,								  //
			SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
}

void DBSystem::ShowButton(DBButtonId Id)
{
	DBButtonBase Button;
	if (Buttons.FindByIndex(Id, Button))
	{
		SetWindowPos(Button.Window, 0,			  //
			Button.Position.X, Button.Position.Y, //
			Button.Size.X, Button.Size.Y,		  //
			SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
}

bool DBSystem::IsPortraitModeEnabled()
{
	return WindowSize.X < WindowSize.Y;
}

void DBSystem::SetFontSize(HWND Window, int Size)
{
	LOGFONT logfont;
	ZeroMemory(&logfont, sizeof(LOGFONT));
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfHeight  = -(Size);
	HFONT hFont		  = CreateFontIndirect(&logfont);

	SendMessage(Window, WM_SETFONT, (WPARAM)hFont, TRUE);
}