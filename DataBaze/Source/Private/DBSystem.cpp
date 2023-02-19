
#include "DBSystem.h"
#include "DBWindowsManager.h"
#include "DBDataManager.h"
#include "DBFunctionLibrary.h"

#include "DBWindowViewer.h"
#include "DBWindowWriter.h"

UINT		 TimerId;
int			 clicks;
const Size2D MainBtnSizes = {150, 30};
DBWindow	 FolderText;
DBWindow	 ListLen;

DBSystem::DBSystem(HINSTANCE HInstance, HWND InMainWindow)
	: HIns(HInstance)
	, MainWindow(InMainWindow)
{
	ThisObj = this;

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

	InitListBox();

	FolderText.Id		= EDBWinCompId::IDC_FolderId;
	FolderText.Parent	= MainWindow;
	FolderText.Position = {245, 320};
	FolderText.Size		= {55, 30};
	FolderText.Text		= L"1";
	FolderText.FontSize = 20;
	FolderText.HIns		= GetModuleHandle(NULL);

	DBLib::CreateStaticBox(FolderText, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER);

	ListLen.Id		 = EDBWinCompId::IDC_ListLength;
	ListLen.Parent	 = MainWindow;
	ListLen.Position = {350, 320};
	ListLen.Size	 = {70, 30};
	ListLen.Text	 = L"500";
	ListLen.FontSize = 20;
	ListLen.HIns	 = GetModuleHandle(NULL);

	DBLib::CreateStaticBox(ListLen, WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER);
	UpdateCount();
}

DBInterface* DBSystem::GetSystem() const
{
	return Cast<DBInterface>(ThisObj);
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
	UpdateCount();
}

void DBSystem::CreateListBox()
{
	ListBox = CreateWindow(L"LISTBOX", L"button", WS_VISIBLE | WS_CHILD | (LBS_NOTIFY | LBS_SORT | WS_VSCROLL), 200, 25, 500, 300,
		MainWindow, (HMENU)IDC_LISTBOX, NULL, NULL);
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
	if (Enabled)
	{
		SetWindowPos(MainWindow, HWND_TOP, 25, 25, 0, 0, SWP_NOSIZE);
	}

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
	SetWindowPos(FolderText.Window, 0, FolderText.Position.X + Offset2, FolderText.Position.Y, 0, 0, SWP_NOSIZE);
	SetWindowPos(ListLen.Window, 0, ListLen.Position.X + Offset2, ListLen.Position.Y, 0, 0, SWP_NOSIZE);
	SetWindowPos(ListBox, HWND_TOP, 200 + Offset2, 25, 500 + (int)((float)Offset2 * 0.5f), 280, 0);
}

void DBSystem::UpdateCount()
{
	ListItemCount = SendMessage(ListBox, LB_GETCOUNT, 0, 0);
	DBLib::SetText(ListLen.Window, std::to_wstring(ListItemCount));
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
				// int lbItem = (int)SendMessage(ListBox, LB_GETCURSEL, 0, 0);
				// Get item data.
				// int i = (int)SendMessage(ListBox, LB_GETITEMDATA, lbItem, 0);

				DBFamilyData SelectedData;
				if (! DataManager->LoadMember(SelectedData))
				{
					assert(false && "Cannot load member by index - System");
					return;
				}
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
		if (WindowManager && ListItemCount < MAX_MEMBERS_NUM)
		{
			WindowManager->OpenWindowByType(EWindows::IDW_WRITER);
		}
		return;
	}
	case IDC_LOCK:
	{
		DBDebug::CreateMessageBox("Lock does not work");
		return;
	}
	case IDC_UNLOCK:
	{
		++clicks;
		KillTimer(NULL, TimerId);
		TimerId = SetTimer(NULL, 0, 200, &DBSystem::DoubleClickTimer);

		if (clicks == 2)
		{
			DBDebug::CreateMessageBox("Unlock double clicked");
		}
		return;
	}
	case IDC_Minimize:
	{
		SetMinimizedMode(! MinimizedMode);
		break;
	}
	case IDC_NextFolder:
	{
		if (DataManager->ChangeFolder(true))
		{
			std::wstring IdAsText = std::to_wstring(DataManager->GetFolderId());
			DBLib::SetText(FolderText.Window, IdAsText);
			InitListBox();
		}
		break;
	}
	case IDC_PrevFolder:
	{
		if (DataManager->ChangeFolder(false))
		{
			std::wstring IdAsText = std::to_wstring(DataManager->GetFolderId());
			DBLib::SetText(FolderText.Window, IdAsText);
			InitListBox();
		}
		break;
	}
	} // switch end
}

void DBSystem::CallPaint(HWND& hWnd, UINT Message, WPARAM& WParam, LPARAM& LParam)
{
	return;
}

void DBSystem::OnMemberAddedHandle()
{
	std::wstring LastMember;
	DataManager->GetLastMemberStatus(LastMember);
	ListData.push_back(LastMember);

	int ItemId = SendMessage(ListBox, LB_ADDSTRING, 0, (LPARAM)LastMember.c_str());
	SendMessage(ListBox, LB_SETITEMDATA, ItemId, ListBoxLastItem);
	++ListBoxLastItem;

	UpdateCount();
}
