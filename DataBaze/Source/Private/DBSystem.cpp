
#include "DBSystem.h"
#include "DBWindowsManager.h"
#include "DBDataManager.h"
#include "DBFunctionLibrary.h"

#include "DBWindowViewer.h"
#include "DBWindowWriter.h"

UINT		 TimerId;
int			 clicks;
const Size2D MainBtnSizes = {150, 30};

DBSystem::DBSystem(HINSTANCE HInstance, HWND InMainWindow)
	: HIns(HInstance)
	, MainWindow(InMainWindow)
{
	WindowManager = CreateComponent<DBWindowsManager>();
	if (WindowManager)
	{
		WindowManager->Initialize(HInstance);
	}
	DataManager = CreateComponent<DBDataManager>();
	if (DataManager)
	{
		DataManager->OnMemberAdded.Bind(this, &DBSystem::OnMemberAddedHandle);
	}
	assert(WindowManager);
	assert(DataManager);
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
	ResetList();
	DataManager->GetMembersList(ListData);
	for (auto& Elem : ListData)
	{
		int ItemId = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)Elem.c_str());
		SendMessage(ListBox, LB_SETITEMDATA, ItemId, ListBoxLastItem);
		++ListBoxLastItem;
	}
}

void DBSystem::CreateListBox()
{
	ListBox = CreateWindow(L"LISTBOX", L"button", WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD | LBS_STANDARD, 200, 25, 500, 300, MainWindow,
		(HMENU)IDC_LISTBOX, NULL, NULL);
	DBLib::SetFontSize(ListBox, 20);
}

void DBSystem::CreateButtons()
{
	struct LocalBtnInfo
	{
		EDBWinCompId Id;
		Size2D		 Pos;
		Size2D		 Size;
		std::wstring Text;
	}; //
	const Size2D	   FolderBtnSize = {45, 30};
	const LocalBtnInfo MainButtons[] = {
		//
		{EDBWinCompId::IDC_VIEW, Size2D(25, 25), MainBtnSizes, L"Show"},			//
		{EDBWinCompId::IDC_NEWITEM, Size2D(25, 75), MainBtnSizes, L"Add New Item"}, //
		{EDBWinCompId::IDC_LOCK, Size2D(25, 125), MainBtnSizes, L"Lock"},			//
		{EDBWinCompId::IDC_UNLOCK, Size2D(25, 175), MainBtnSizes, L"Unlock"},		//
		{EDBWinCompId::IDC_PrevFolder, Size2D(200, 320), FolderBtnSize, L" < "},	//
		{EDBWinCompId::IDC_NextFolder, Size2D(300, 320), FolderBtnSize, L" >"},		//
		{EDBWinCompId::IDC_Minimize, Size2D(655, 320), FolderBtnSize, L" <<"}		//
	};

	// Size2D BtnSize = {150, 30};
	for (auto BtnInfo : MainButtons)
	{
		DBWindow NewWindow(BtnInfo.Id, MainWindow, BtnInfo.Pos, BtnInfo.Size, BtnInfo.Text);
		auto	 Btn = DBLib::CreateButton(NewWindow);

		DBLib::SetFontSize(Btn, 16);
		NewWindow.Window = Btn;
		Buttons.Add(NewWindow);
	}
}

void DBSystem::ResetList()
{
	SendMessage(ListBox, LB_RESETCONTENT, 0, 0);
	ListBoxLastItem = 0;
	ListData.clear();
}

void DBSystem::SetMinimizedMode(bool Enabled)
{
	MinimizedMode = Enabled;

	const int Offset = MinimizedMode ? -300 : 0;
	SetWindowPos(MainWindow, HWND_TOP, 0, 0, 740 + Offset, 450, SWP_NOMOVE);

	const int Offset2 = MinimizedMode ? -190 : 0;
	for (auto nBtn : Buttons.Buttons)
	{
		switch (nBtn.Id)
		{
		case IDC_PrevFolder:
		{
			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset2, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
			break;
		}
		case IDC_NextFolder:
		{
			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset2, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
			break;
		}
		case IDC_Minimize:
		{
			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + (int)((float)Offset * 0.95f), nBtn.Position.Y, 0, 0, SWP_NOSIZE);
			break;
		}
		default:
		{
			SetWindowPos(nBtn.Window, 0, nBtn.Position.X + Offset, nBtn.Position.Y, 0, 0, SWP_NOSIZE);
		}
		} //  switch id
	}
	SetWindowPos(ListBox, HWND_TOP, 200 + Offset2, 25, 500 + (int)((float)Offset2 * 0.5f), 300, 0);
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
			if (WindowManager && DataManager)
			{
				// Get selected index.
				int lbItem = (int)SendMessage(ListBox, LB_GETCURSEL, 0, 0);

				// Get item data.
				int i = (int)SendMessage(ListBox, LB_GETITEMDATA, lbItem, 0);

				DBFamilyData SelectedData;
				DataManager->LoadMember(SelectedData);
				WindowManager->OpenWindowByType(EWindows::IDW_VIEWER);

				if (WindowManager->GetViewer())
				{
					WindowManager->GetViewer()->SetMemberData(SelectedData);
					WindowManager->EndConstruct();
				}
			}
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
			WindowManager->OpenWindowByType(EWindows::IDW_VIEWER);
		}
		return;
	}
	case IDC_NEWITEM:
	{
		if (WindowManager)
		{
			WindowManager->OpenWindowByType(EWindows::IDW_WRITER);
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
	case IDC_Minimize:
	{
		SetMinimizedMode(! MinimizedMode);
		break;
	}
	} // switch end
}

void DBSystem::CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam)
{
	return;
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
	Size2D Pos = (IsPortraitModeEnabled()) ? Size2D(0, 25) : Size2D((MainBtnSizes).X + 50, 25);
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

void DBSystem::OnMemberAddedHandle()
{
	std::wstring LastMember;
	DataManager->GetLastMemberStatus(LastMember);
	ListData.push_back(LastMember);

	int ItemId = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)LastMember.c_str());
	SendMessage(ListBox, LB_SETITEMDATA, ItemId, ListBoxLastItem);
	++ListBoxLastItem;
}
