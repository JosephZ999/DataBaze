#include "DBWindowViewer.h"
#include "DBKeyCodes.h"

#include "Components/DBButtonManager.h"
#include "Components/UIVerticalBox.h"
#include "Components/UIHorizontalBox.h"
#include "UILibrary.h"

LRESULT DBWindowViewer::CallProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			RegisterHotKey(hWnd, HKV_Command_1, MOD_CONTROL, VK_NUMPAD1);
			RegisterHotKey(hWnd, HKV_Command_2, MOD_CONTROL, VK_NUMPAD2);
			RegisterHotKey(hWnd, HKV_Command_3, MOD_CONTROL, VK_NUMPAD3);
			// RegisterHotKey(hWnd, HKV_Command_4, MOD_CONTROL, VK_NUMPAD4);
		}
		break;
	}
	case WM_CLOSE:
	{
		UnregisterHotKey(hWnd, HKV_Command_1);
		UnregisterHotKey(hWnd, HKV_Command_2);
		UnregisterHotKey(hWnd, HKV_Command_3);
		// UnregisterHotKey(hWnd, HKV_Command_4);

		OnClose.Broadcast();
		return 0;
	}

	case WM_HOTKEY:
	{
		switch (wParam)
		{
		case 1: Autofill_Form1(); break;
		case 2: Autofill_Form2(); break;
		case 3: Autofill_Form3(); break;
		case 4: Autofill_Form4(); break;
		}
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC			hdc = BeginPaint(hWnd, &ps);

		Size2D WindowSize = DBLib::GetScreenSize(hWnd);
		RECT   Rect		  = {0, 0, WindowSize.X, WindowSize.Y};
		HBRUSH BrushColor = CreateSolidBrush(RGB(200, 220, 200));
		FillRect(hdc, &Rect, BrushColor);

		EndPaint(hWnd, &ps);
		break;
	}
	}

	EDBWinCompId BtnId = static_cast<EDBWinCompId>(LOWORD(wParam));
	switch (BtnId)
	{
	case EDBWinCompId::IDC_V_Edit:
	{
		cmd::wnd::WriterEditMember(SelectedMemberId, SelectedFolderId, CurrentPeople, MemberData);
		break;
	}
	case EDBWinCompId::IDC_V_Prev:
	{
		ChangePeople(false);
		break;
	}
	case EDBWinCompId::IDC_V_Next:
	{
		ChangePeople(true);
		break;
	}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowViewer::DBWindowViewer(HWND OwningWnd)
{
	WindowHandle = OwningWnd;

	ButtonManager = CreateComponent<DBButtonManager>();
	if (ButtonManager)
	{
		auto VBox = UILib::CreateVerticalBox(Size2D(25, 25), Size2D(535, 350));

		// Info boxes
		{
			auto HBox1 = UILib::CreateHorizontalBox(Size2D(), Size2D());
			HBox1->SetFill(true);

			FWndItem SlotElem1(WT_Static, IDC_V_InfoTitle, Size2D(200, 300), L"", 18, SS_RIGHT);
			FWndItem SlotElem2(WT_Edit, IDC_V_Info, Size2D(335, 300), L"", 18, ES_MULTILINE | ES_READONLY);
			HWND	 SlotWnd1 = ButtonManager->AddItem(WindowHandle, SlotElem1);
			HWND	 SlotWnd2 = ButtonManager->AddItem(WindowHandle, SlotElem2);

			auto Slot1 = UILib::CreateSlot(SlotWnd1, SlotElem1.Size);
			Slot1->SetHAlign(HA_Left);
			Slot1->SetFill(false);

			auto Slot2 = UILib::CreateSlot(SlotWnd2, SlotElem2.Size);

			HBox1->AddChild(Slot1);
			// HBox1->AddChild(UILib::CreateSpacer(Size2D(10, 0)));
			HBox1->AddChild(Slot2);
			VBox->AddChild(HBox1);
		}

		// Buttons
		{
			auto HBox2 = UILib::CreateHorizontalBox(Size2D(), Size2D(0, 35));
			HBox2->SetFill(false);
			HBox2->SetVAlign(VA_Bottom);

			FWndItem SlotElem1(WT_Button, IDC_V_Prev, Size2D(100, 35), L"< Prev");
			FWndItem SlotElem2(WT_Button, IDC_V_Edit, Size2D(100, 35), L"Edit");
			FWndItem SlotElem3(WT_Button, IDC_V_Next, Size2D(100, 35), L"Next >");
			HWND	 SlotWnd1 = ButtonManager->AddItem(WindowHandle, SlotElem1);
			HWND	 SlotWnd2 = ButtonManager->AddItem(WindowHandle, SlotElem2);
			HWND	 SlotWnd3 = ButtonManager->AddItem(WindowHandle, SlotElem3);

			auto Slot1 = UILib::CreateSlot(SlotWnd1, SlotElem1.Size);
			Slot1->SetVAlign(VA_Bottom);
			Slot1->SetHAlign(HA_Left);

			auto Slot2 = UILib::CreateSlot(SlotWnd2, SlotElem2.Size);
			Slot2->SetVAlign(VA_Bottom);
			Slot2->SetHAlign(HA_Center);

			auto Slot3 = UILib::CreateSlot(SlotWnd3, SlotElem3.Size);
			Slot3->SetVAlign(VA_Bottom);
			Slot3->SetHAlign(HA_Right);

			HBox2->AddChild(Slot1);
			HBox2->AddChild(Slot2);
			HBox2->AddChild(Slot3);
			VBox->AddChild(UILib::CreateSpacer(Size2D(15, 15)));
			VBox->AddChild(HBox2);
		}

		ButtonManager->AddSlot(VBox);
	}
}

void DBWindowViewer::SetMemberData(int MemberId, int FolderId, const DBFamilyData& InData)
{
	SelectedMemberId = MemberId;
	SelectedFolderId = FolderId;
	MemberData		 = InData;
	PrintData();
}

void DBWindowViewer::Autofill_Form1()
{
	if (MemberData.Parents.size() == 0) return;

	PasteString(MemberData.Parents[0].Name);
	DBInput::PressKey(VK_TAB);

	PasteString(MemberData.Parents[0].FamilyName);
	DBInput::PressKey(VK_RETURN);

	PasteString(std::to_string(MemberData.Parents[0].BirthMonth));
	DBInput::PressKey(VK_TAB);

	PasteString(std::to_string(MemberData.Parents[0].BirthDay));
	DBInput::PressKey(VK_TAB);

	PasteString(std::to_string(MemberData.Parents[0].BirthYear));
	DBInput::PressKey(VK_RETURN);

	PasteString(MemberData.MailStreet);
	DBInput::PressKey(VK_RETURN);

	PasteString(MemberData.MailHomeNumber);
	DBInput::PressKey(VK_RETURN);

	PasteString(MemberData.MailCity);
	DBInput::PressKey(VK_RETURN);

	PasteString(MemberData.MailRegion);
	DBInput::PressKey(VK_RETURN);

	PasteString(MemberData.MailCountry);
	DBInput::PressKey(VK_RETURN);
}

void DBWindowViewer::Autofill_Form2()
{
	DBDebug::CreateMessageBox("Fill 2");
}

void DBWindowViewer::Autofill_Form3() {}

void DBWindowViewer::Autofill_Form4() {}

void DBWindowViewer::ChangePeople(bool Next)
{
	const bool HasSpouse = MemberData.Parents.size() > 1;
	const bool HasChild	 = MemberData.Children.size() > 0;
	if (													   //
		(! Next && CurrentPeople == EPeopleType::PT_Parent) || //
		(Next && ! HasSpouse && ! HasChild)					   //
	)
	{
		return;
	}

	bool		bChanged = false;
	EPeopleType NextP	 = (EPeopleType)((int)CurrentPeople + (Next ? 1 : -1));
	switch (NextP)
	{
	case PT_Parent:
	{
		CurrentPeople = NextP;
		bChanged	  = true;
		break;
	}
	case PT_Spouse:
	{
		if (HasSpouse)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		else if (HasChild)
		{
			CurrentPeople = Next ? PT_Child_1 : PT_Parent;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_1:
	{
		if (MemberData.Children.size() >= 1)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_2:
	{
		if (MemberData.Children.size() >= 2)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_3:
	{
		if (MemberData.Children.size() >= 3)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_4:
	{
		if (MemberData.Children.size() >= 4)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_5:
	{
		if (MemberData.Children.size() >= 5)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_6:
	{
		if (MemberData.Children.size() >= 6)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_7:
	{
		if (MemberData.Children.size() >= 7)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_8:
	{
		if (MemberData.Children.size() >= 8)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	case PT_Child_9:
	{
		if (MemberData.Children.size() >= 9)
		{
			CurrentPeople = NextP;
			bChanged	  = true;
		}
		break;
	}
	}

	if (bChanged)
	{
		PrintData();
	}
}

void DBWindowViewer::PrintData()
{
	if (MemberData.Parents.size() == 0) return;

	// Clear text
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_InfoTitle), L"");
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_Info), L"");
	Title.clear();
	Info.clear();

	HasSpouse = MemberData.Parents.size() > 1;
	HasChild  = MemberData.Children.size() > 0;

	PrintPeople(MemberData[CurrentPeople], false);

	if (true)
	{
		PrintMail();
	}

	// convert to wstr
	std::wstring InfoAsWStr;
	DBConvert::StringToWString(Info, InfoAsWStr);

	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_InfoTitle), Title);
	DBLib::SetText(ButtonManager->GetWndHandler(IDC_V_Info), InfoAsWStr);
}

void DBWindowViewer::PrintPeople(const DBPeopleData& People, bool IsChild)
{
	std::vector<FillData> DataArray;

	DataArray.push_back({L"Name : ", People.Name});
	DataArray.push_back({L"Family Name : ", People.FamilyName});
	DataArray.push_back({L"Birth Data (d/m/y) : ", People.GetBirthDataAsString()});
	DataArray.push_back({L"Gender : ", People.GetGenderAsString()});
	DataArray.push_back({L"Birth Country : ", People.BirthCountry});

	if (! IsChild)
	{
		DataArray.push_back({L"Ñountry Wher Live : ", People.WhereLive});
		DataArray.push_back({L"Education : ", People.GetEducationAsString()});
	}
	DataArray.push_back({L"Image Path : ", std::string(People.ImageFile).erase(0, 6)});

	for (auto Elem : DataArray)
	{
		Title.append(Elem.Title).append(L"\n");
		Info.append(Elem.Info).append("\r\n");
	}
}

void DBWindowViewer::PrintMail()
{
	std::vector<FillData> DataArray;

	DataArray.push_back({L"Mail Country : ", MemberData.MailCountry});
	DataArray.push_back({L"Mail Region : ", MemberData.MailRegion});
	DataArray.push_back({L"Mail City : ", MemberData.MailCity});
	DataArray.push_back({L"Mail Street : ", MemberData.MailStreet});
	DataArray.push_back({L"Mail Home : ", MemberData.MailHomeNumber});
	DataArray.push_back({L"Mail Code : ", std::to_string(MemberData.MailZipCode)});

	for (auto Elem : DataArray)
	{
		Title.append(Elem.Title).append(L"\n");
		Info.append(Elem.Info).append("\r\n");
	}
}

inline void DBWindowViewer::PasteString(const std::string Text)
{
	Sleep(20);
	DBInput::CopyToClipboard(WindowHandle, Text);
	DBInput::PasteClipboard();
}
