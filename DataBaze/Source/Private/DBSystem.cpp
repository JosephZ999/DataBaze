
#include "DBSystem.h"
#include "DBWindowsManager.h"
#include "DBDataManager.h"
#include "DBFunctionLibrary.h"

UINT TimerId;
int	 clicks;

DBSystem::DBSystem(HINSTANCE HInstance, HWND InMainWindow)
	: HIns(HInstance)
	, MainWindow(InMainWindow)
{
	WindowManager = new DBWindowsManager(HIns);
	WindowManager->SetOwner(this);

	DataManager = new DBDataManager;
	DataManager->SetOwner(this);
}

DBSystem::~DBSystem()
{
	delete WindowManager;
	WindowManager = nullptr;

	delete DataManager;
	DataManager = nullptr;
}

void DBSystem::EndConstruct()
{
	DataManager->OnUpdate.Bind(this, &DBSystem::OnDataUpdated);

	CreateListBox();
	CreateButtons();

	// DataManager->LoadFiles();
	if (DataManager->SearchValidFolders())
	{
		InitListBox();
	}
}

DBInterface* DBSystem::GetSystem()
{
	return this;
}

void DBSystem::InitListBox()
{
	DataManager->GetMembersList(ListData);

	ResetList();
	for (auto& Elem : ListData)
	{
		int ItemId = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)Elem.c_str());
		SendMessage(ListBox, LB_SETITEMDATA, ItemId, ListBoxLastItem);
		++ListBoxLastItem;
	}
}

void DBSystem::OnDataUpdated() {}

void DBSystem::CreateListBox()
{
	// Init ListContainer
	ListContainer.Items.push_back(DBFamilyData({DBPeopleData(L"Abdu")}, {}, false));
	ListContainer.Items.push_back(DBFamilyData({DBPeopleData(L"Amir")}, {}, false));
	ListContainer.Items.push_back(DBFamilyData({DBPeopleData(L"Piyoz")}, {}, false));
	ListContainer.Items.push_back(DBFamilyData({DBPeopleData(L"Hechkim")}, {}, false));
	ListContainer.Items.push_back(DBFamilyData({DBPeopleData(L"Komp")}, {}, false));

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
	DBLib::SetFontSize(ListBox, 20);
}

void DBSystem::CreateButtons()
{
	struct LocalBtnInfo
	{
		EDBWinCompId Id;
		Size2D		 Pos;
		std::wstring Text;
	};

	const LocalBtnInfo MainButtons[] = {
		//
		{EDBWinCompId::IDC_VIEW, Size2D(25, 25), L"Show"},			  //
		{EDBWinCompId::IDC_NEWITEM, Size2D(25, 75), L"Add New Item"}, //
		{EDBWinCompId::IDC_LOCK, Size2D(25, 125), L"Lock"},			  //
		{EDBWinCompId::IDC_UNLOCK, Size2D(25, 175), L"Unlock"}		  //
	};

	// Size2D BtnSize = {150, 30};
	for (auto BtnInfo : MainButtons)
	{
		DBWindow NewWindow(BtnInfo.Id, MainWindow, BtnInfo.Pos, BtnSize, BtnInfo.Text);
		auto	 Btn = DBLib::CreateButton(NewWindow);
		if (Btn)
		{
			DBLib::SetFontSize(Btn, 16);
			NewWindow.Window = Btn;
			Buttons.Add(NewWindow);
		}
	}
}

void DBSystem::ResetList()
{
	SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
	ListBoxLastItem = 0;
}

VOID DBSystem::DoubleClickTimer(HWND hWnd, UINT nMsg, UINT_PTR nIDEvent, DWORD dwTime)
{
	KillTimer(NULL, TimerId);
	clicks = 0;
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

			std::wstring MyMessage;
			MyMessage = std::to_wstring(i);

			MessageBox(NULL, MyMessage.c_str(), L"Dialog Box", MB_OK);
			return;
		}
		case LBN_SELCHANGE:
		{
			// Get selected index.
			int lbItem = (int)SendMessage(ListBox, LB_GETCURSEL, 0, 0);

			// Get item data.
			int i = (int)SendMessage(ListBox, LB_GETITEMDATA, lbItem, 0);

			DataManager->SelectMember(i);
			return;
		}
		} // switch end
	}
	} // switch end

	EDBWinCompId BtnId = static_cast<EDBWinCompId>(LOWORD(WParam));
	switch (BtnId)
	{
	case IDC_VIEW:
	{
		if (WindowManager)
		{
			WindowManager->OpenWindow(EWindows::IDW_VIEWER);
		}
		return;
	}
	case IDC_NEWITEM:
	{
		if (WindowManager)
		{
			WindowManager->OpenWindow(EWindows::IDW_WRITER);
		}
		return;
	}
	case IDC_LOCK:
	{
		MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		return;
	}
	case IDC_UNLOCK:
	{
		++clicks;
		KillTimer(NULL, TimerId);
		TimerId = SetTimer(NULL, 0, 200, &DBSystem::DoubleClickTimer);

		if (clicks == 2)
		{
			MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		}
		return;
	}
	} // switch end

	// Windows messages

	// auto WId = static_cast<EWindows>(LOWORD(WParam));
	// switch (WId)
	//{
	// case IDW_VIEWER:
	//{
	//	WindowManager->CallCommand(hWnd, Message, WParam, LParam);
	//	return;
	//}
	// case IDW_WRITER:
	//{
	//	WindowManager->CallCommand(hWnd, Message, WParam, LParam);
	//	return;
	//}
	//} // switch end
}

void DBSystem::CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam)
{
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

	for (auto Btn : Buttons.Buttons)
	{
		DBLib::SetWindowVisibility(Btn.Window, ! IsPortraitModeEnabled());
	}
}

void DBSystem::Update_ListBoxScale()
{
	Size2D Pos = (IsPortraitModeEnabled()) ? Size2D(0, 25) : Size2D((BtnSize).X + 50, 25);
	Size2D Size;
	Size.X = abs(Pos.X - (WindowSize.X - 25));
	Size.Y = WindowSize.Y - 100;

	SetWindowPos(ListBox, 0, //
		Pos.X, Pos.Y,		 //
		Size.X, Size.Y,		 //
		1);
}

void DBSystem::HideButton(EDBWinCompId Id)
{
	DBWindow Button;
	if (Buttons.FindByIndex(Id, Button))
	{
		SetWindowPos(Button.Window, 0,			  //
			Button.Position.X, Button.Position.Y, //
			0, 0,								  //
			SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	}
}

void DBSystem::ShowButton(EDBWinCompId Id)
{
	DBWindow Button;
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
