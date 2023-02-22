#include "DBWindowViewer.h"
#include "DBFunctionLibrary.h"
#include <Windows.h>
#include "DBKeyCodes.h"
#include "DBWindowsManager.h"

DBWindowViewer* ViewerObj = nullptr;
DBWindow		ViewerInfo;
DBWindow		ViewerInfoTitle;

LRESULT CALLBACK WndViewerProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		ViewerInfoTitle.Id		 = EDBWinCompId::IDC_V_InfoTitle;
		ViewerInfoTitle.Parent	 = hWnd;
		ViewerInfoTitle.Position = {25, 25};
		ViewerInfoTitle.Size	 = {265, 300};
		ViewerInfoTitle.Text	 = L"Name : ";
		ViewerInfoTitle.FontSize = 18;
		ViewerInfoTitle.HIns	 = GetModuleHandle(NULL);

		DBLib::CreateStaticBox(ViewerInfoTitle, WS_VISIBLE | WS_CHILD | SS_RIGHT);

		ViewerInfo.Id		= EDBWinCompId::IDC_V_Info;
		ViewerInfo.Parent	= hWnd;
		ViewerInfo.Position = {290, 25};
		ViewerInfo.Size		= {265, 300};
		ViewerInfo.Text		= L"Moon";
		ViewerInfo.FontSize = 18;
		ViewerInfo.HIns		= GetModuleHandle(NULL);

		DBLib::CreateStaticBox(ViewerInfo, WS_VISIBLE | WS_CHILD);
		break;
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
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

		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}

	case WM_HOTKEY:
	{
		if (! ViewerObj) break;

		switch (wParam)
		{
		case 1: ViewerObj->Autofill_Form1(); break;
		case 2: ViewerObj->Autofill_Form2(); break;
		case 3: ViewerObj->Autofill_Form3(); break;
		case 4: ViewerObj->Autofill_Form4(); break;
		}
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
	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowViewer::DBWindowViewer(DBInterface* InOwner)
{
	SetOwner(InOwner);
	auto Manager = Cast<DBWindowsManager>(GetOwner());
	if (Manager)
	{
		WindowHandle = Manager->GetViewerHandle();
		ViewerObj	 = this;
	}
}

void DBWindowViewer::OnConstruct()
{
	PrintData();
}

void DBWindowViewer::SetMemberData(const DBFamilyData& InData)
{
	MemberData = InData;
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
}

void DBWindowViewer::Autofill_Form2()
{
	DBDebug::CreateMessageBox("Fill 2");
}

void DBWindowViewer::Autofill_Form3() {}

void DBWindowViewer::Autofill_Form4() {}

void DBWindowViewer::PrintData()
{
	if (MemberData.Parents.size() == 0) return;

	// Clear text
	DBLib::SetText(ViewerInfoTitle.Window, L"");
	DBLib::SetText(ViewerInfo.Window, L"");

	HasSpouse = MemberData.Parents.size() > 1;
	HasChild  = MemberData.Children.size() > 0;

	PrintPeople(MemberData.Parents[0], false);

	if (true)
	{
		PrintMail();
	}

	// convert to wstr
	std::wstring InfoAsWStr;
	DBConvert::StringToWString(Info, InfoAsWStr);

	DBLib::SetText(ViewerInfoTitle.Window, Title);
	DBLib::SetText(ViewerInfo.Window, InfoAsWStr);
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
		DataArray.push_back({L"Ñountry Wher Live: ", People.WhereLive});
		DataArray.push_back({L"Education : ", People.GetEducationAsString()});
	}
	DataArray.push_back({L"Image Path : ", std::string(People.ImageFile).erase(0, 6)});

	for (auto Elem : DataArray)
	{
		Title.append(Elem.Title).append(L"\n");
		Info.append(Elem.Info).append("\n");
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
		Info.append(Elem.Info).append("\n");
	}
}

inline void DBWindowViewer::PasteString(const std::string Text)
{
	Sleep(20);
	DBInput::CopyToClipboard(WindowHandle, Text);
	DBInput::PasteClipboard();
}
