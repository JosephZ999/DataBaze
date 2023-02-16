
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"
#include <commdlg.h>
#include <comdef.h>
#include "DBWindowsManager.h"
#include "DBDataManager.h"

#define EDIT_STYLE_BASE WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN

DBWindowWriter* WriterObj = nullptr;
DBWindow		WriterEditBox;
DBWindow		WriterInfoBox;

LRESULT CALLBACK WndWriterProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		WriterEditBox.Id	   = EDBWinCompId::IDC_W_Edit;
		WriterEditBox.Parent   = hWnd;
		WriterEditBox.Position = {25, 150};
		WriterEditBox.Size	   = {520, 40};
		WriterEditBox.Text	   = {};
		WriterEditBox.FontSize = 32;
		WriterEditBox.HIns	   = GetModuleHandle(NULL);

		DBLib::CreateEditBox(WriterEditBox, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_CENTER | ES_UPPERCASE | ES_WANTRETURN);

		WriterInfoBox.Id	   = EDBWinCompId::IDC_W_Info;
		WriterInfoBox.Parent   = hWnd;
		WriterInfoBox.Position = {25, 25};
		WriterInfoBox.Size	   = {520, 120};
		WriterInfoBox.Text	   = L"Info Info Info Info Info Info Info Info Info Info Info Info Info Info";
		WriterInfoBox.FontSize = 24;
		WriterInfoBox.HIns	   = GetModuleHandle(NULL);

		DBLib::CreateStaticBox(WriterInfoBox, WS_VISIBLE | WS_CHILD );
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (! WriterObj) break;

			SetFocus(WriterEditBox.Window);

			WriterObj->SelectWriteData(WriterObj->PeopleType);
			break;
		}
		HDC hdc2 = GetDC(WriterInfoBox.Window);
		SetTextColor(hdc2, RGB(255, 255, 255));
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);

		Size2D WindowSize = DBLib::GetScreenSize(hWnd);
		RECT   Rect		  = {0, 0, WindowSize.X, WindowSize.Y};
		HBRUSH BrushColor = CreateSolidBrush(RGB(200, 200, 220));
		FillRect(hdc, &Rect, BrushColor);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_KEYDOWN:
	{
		// MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		break;
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		return 0;
	}
	case WM_HOTKEY:
	{
		if (! WriterObj) break;

		if (WriterObj->CheckFormat())
		{
			WriterObj->WriteData();
			WriterObj->SelectWriteData(WriterObj->PeopleType);
		}

		SendMessage(WriterEditBox.Window, WM_SETTEXT, 0, (LPARAM)L"");
		break;
	}
	} // switch end

	// Edit control options
	switch (LOWORD(wParam))
	{
	case IDC_W_Edit:
	{
		if (HIWORD(wParam) == EN_SETFOCUS)
		{
			RegisterHotKey(hWnd, HKW_Enter, 0, VK_RETURN);
		}
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			UnregisterHotKey(hWnd, HKW_Enter);
		}
		break;
	}
	}

	/*
	char str[30];
	sprintf_s(str, sizeof(str), "Unfocus");
	OutputDebugStringA(str);
	*/

	return DefWindowProc(hWnd, message, wParam, lParam);
}

DBWindowWriter::DBWindowWriter(DBInterface* InOwner)
{
	SetOwner(InOwner);
	// auto Manager = static_cast<DBWindowsManager*>(GetOwner());
	auto Manager = Cast<DBWindowsManager>(GetOwner());
	if (Manager)
	{
		WindowHandle = Manager->GetWriterHandle();
		WriterObj	 = this;
	}
}

void DBWindowWriter::OnConstruct()
{
	// we can reset all data
}

void DBWindowWriter::SelectWriteData(EPeopleType PT)
{
	if (Finish) return;

	switch (PT)
	{
	case PT_Parent:
	{
		if (MembersData.Parents.size() < 1)
		{
			MembersData.Parents.push_back(DBPeopleData());
		}
		DataToChange = &MembersData.Parents[0];
		break;
	}
	case PT_Spouse:
	{
		if (Status != EMeritialStatus::MS_Married)
		{
			NextPeople();
			break;
		}

		if (MembersData.Parents.size() < 2)
		{
			MembersData.Parents.push_back(DBPeopleData());
		}
		DataToChange = &MembersData.Parents[1];
		break;
	}
	case PT_Child_1: SelectChild(1); break;
	case PT_Child_2: SelectChild(2); break;
	case PT_Child_3: SelectChild(3); break;
	case PT_Child_4: SelectChild(4); break;
	case PT_Child_5: SelectChild(5); break;
	case PT_Child_6: SelectChild(6); break;
	case PT_Child_7: SelectChild(7); break;
	case PT_Child_8: SelectChild(8); break;
	case PT_Child_9: SelectChild(9); break;
	}
	WriterObj->UpdateInfo();
	WriterObj->UpdateEditStyle();
}

void DBWindowWriter::WriteData()
{
	if (! DataToChange) return;

	TCHAR buff[256];
	GetWindowText(WriterEditBox.Window, buff, 256);

	std::string OutValue;
	DBConvert::WStringToString(buff, OutValue);
	SetItem(OutValue);

	NextLine();
	switch (PeopleType)
	{
	case PT_Parent:
	{
		switch (PeopleData)
		{
		case PD_NotChildInfo: NextLine(); break;
		case PD_OnlyParentInfo: NextLine(); break;
		case PD_Max:
		{
			FinishWriting();
			NextPeople();
			break;
		}
		}
		break;
	}
	case PT_Spouse:
	{
		switch (PeopleData)
		{
		case PD_NotChildInfo: NextLine(); break;
		case PD_OnlyParentInfo:
		{
			FinishWriting();
			NextPeople();
			break;
		}
		} // switch
		break;
	}
	} // switch

	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (PeopleData == PD_ImageFile)
	{
		OpenImage();
		if (IsParent || IsSpouse)
		{
			NextLine();
			NextLine();
		}
		else
		{
			++EnteredChildrenNum;
			FinishWriting();
			NextPeople();
		}
	}
}

void DBWindowWriter::UpdateInfo()
{
	std::wstring InfoText;

	switch (PeopleType)
	{ // clang-format off
	case PT_Parent:		InfoText.append(L"Parent 1\n\n");	break;
	case PT_Spouse:		InfoText.append(L"Parent 2\n\n");	break;
	case PT_Child_1:	InfoText.append(L"Child 1\n\n");	break;
	case PT_Child_2:	InfoText.append(L"Child 2\n\n");	break;
	case PT_Child_3:	InfoText.append(L"Child 3\n\n");	break;
	case PT_Child_4:	InfoText.append(L"Child 4\n\n");	break;
	case PT_Child_5:	InfoText.append(L"Child 5\n\n");	break;
	case PT_Child_6:	InfoText.append(L"Child 6\n\n");	break;
	case PT_Child_7:	InfoText.append(L"Child 7\n\n");	break;
	case PT_Child_8:	InfoText.append(L"Child 8\n\n");	break;
	case PT_Child_9:	InfoText.append(L"Child 9\n\n");	break;
	} // clang-format on

	switch (PeopleData)
	{ // clang-format off
	case PD_Name:				InfoText.append(L"Name");						break;
	case PD_FamilyName:			InfoText.append(L"Family name");				break;
	case PD_Gender:				InfoText.append(L"Gender");						break;
	case PD_BirthMonth:			InfoText.append(L"Birth Month");				break;
	case PD_BirthDay:			InfoText.append(L"Birth Day");					break;
	case PD_BirthYear:			InfoText.append(L"Birth Year");					break;
	case PD_BornCountry:		InfoText.append(L"Country where born");			break;
	case PD_EducationDegree:	InfoText.append(L"Education");					break;
	case PD_WhereLive:			InfoText.append(L"Country where live today");	break;
	case PD_MaritalStatus:		InfoText.append(L"Marital Status");				break;
	case PD_ChildrenNum:		InfoText.append(L"Children Num");				break;
	case PD_MailCountry:		InfoText.append(L"Mailing country");			break;
	case PD_MailCity:			InfoText.append(L"Mailing city");				break;
	case PD_MailStreet:			InfoText.append(L"Mailing Street");				break;
	case PD_MailHomeNumber:		InfoText.append(L"Mailing home number");		break;
	case PD_MailZipCode:		InfoText.append(L"Mailing ZipCode");			break;
	} // clang-format on

	SetInfoText(InfoText);
}

void DBWindowWriter::SelectChild(size_t Index)
{
	if (Index > ChildrenNum)
	{
		NextPeople();
		return;
	}

	if (MembersData.Children.size() < Index)
	{
		MembersData.Children.push_back(DBPeopleData());
	}
	DataToChange = &MembersData.Children[Index - 1];
}

void DBWindowWriter::SetInfoText(std::wstring& Text)
{
	SendMessage(WriterInfoBox.Window, WM_SETTEXT, 0, (LPARAM)Text.c_str());
}

bool DBWindowWriter::CheckFormat()
{
	TCHAR buff[256];
	GetWindowText(WriterEditBox.Window, buff, 256);
	std::string Text;
	DBConvert::WStringToString(buff, Text);

	if (Text.size() == 0) return false;

	return true;
}

void DBWindowWriter::UpdateEditStyle()
{
	switch (PeopleData)
	{	// clang-format off
	case PD_Gender:				SetEditboxStyle(ES_NUMBER, 1); break;
	case PD_BirthMonth:			SetEditboxStyle(ES_NUMBER, 2); break;
	case PD_BirthDay:			SetEditboxStyle(ES_NUMBER, 2); break;
	case PD_BirthYear:			SetEditboxStyle(ES_NUMBER, 4); break;
	case PD_EducationDegree:	SetEditboxStyle(ES_NUMBER, 1); break;
	case PD_MaritalStatus:		SetEditboxStyle(ES_NUMBER, 1); break;
	case PD_ChildrenNum:		SetEditboxStyle(ES_NUMBER, 2); break;
	case PD_MailZipCode:		SetEditboxStyle(ES_NUMBER); break;
	default:					SetEditboxStyle(); break;
		// clang-format on
	}	// Switch PeopleData
}

void DBWindowWriter::NextPeople()
{
	PeopleData = EPeopleData::PD_Name;
	PeopleType = static_cast<EPeopleType>(PeopleType + 1);
}

void DBWindowWriter::NextLine()
{
	PeopleData = static_cast<EPeopleData>(PeopleData + 1);
}

void DBWindowWriter::OpenImage()
{
	// Display the Open dialog box.

	bool ImageCopied = false;
	while (! ImageCopied)
	{
		OPENFILENAME ofn;		  // common dialog box structure
		wchar_t		 szFile[512]; // buffer for file name
		// HANDLE    hf;	      // file handle

		// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner	= WindowHandle;
		ofn.lpstrFile	= (LPWSTR)szFile;
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0]	= '\0';
		ofn.nMaxFile		= sizeof(szFile);
		ofn.lpstrFilter		= L"JPEG\0*.JP*G";
		ofn.nFilterIndex	= 1;
		ofn.lpstrFileTitle	= NULL;
		ofn.nMaxFileTitle	= 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags			= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// CDN_FILEOK

		if (GetOpenFileName(&ofn) == TRUE)
		{
			//	hf = CreateFile(ofn.lpstrFile,	 // File Name
			//	GENERIC_READ,				 //
			//	0,							 //
			//	(LPSECURITY_ATTRIBUTES)NULL, //
			//	OPEN_EXISTING,				 //
			//	FILE_ATTRIBUTE_NORMAL,		 //
			//	(HANDLE)NULL);

			ImagePath = ofn.lpstrFile;
			MessageBox(NULL, ImagePath.c_str(), L"Dialog Box", MB_OK);

			ImageCopied = CopyImage();
		}
	}
}

bool DBWindowWriter::CopyImage()
{
	assert(DataToChange);

	if (! GetSystem()) return false;
	auto DataManager = GetSystem()->GetComponent<DBDataManager>();

	if (! DataManager) return false;

	std::wstring FilePath  = DataManager->GenerateImagePath();
	std::wstring FileName  = DataManager->GenerateImageName();
	std::wstring FinalPath = FilePath.append(FileName);

	std::wstring NewPath		   = FinalPath;
	int			 SizeToProjectPath = DBPaths::GetProjectPath().size();
	NewPath.erase(0, SizeToProjectPath);
	std::string NewStringPath;

	DBConvert::WStringToString(NewPath, NewStringPath);
	DataToChange->ImageFile = NewStringPath;

	MessageBox(NULL, FinalPath.c_str(), L"Dialog Box", MB_OK);

	if (CopyFile(ImagePath.c_str(), FinalPath.c_str(), true))
	{
		return true;
	}
	else
	{
		std::wstring ErrorCode(L"Error Code: ");
		ErrorCode.append(std::to_wstring(GetLastError()));
		OutputDebugString(ErrorCode.c_str());
		MessageBox(NULL, L"Error", L"Dialog Box", MB_OK);
		// ERROR_PATH_NOT_FOUND //  error codes
		return false;
	}
	return false;
}

void DBWindowWriter::SetItem(std::string& Info)
{
	if (! DataToChange) return;

	switch (PeopleData)
	{

	case PD_Name:
	{
		DataToChange->Name = Info;
		break;
	}
	case PD_FamilyName:
	{
		DataToChange->FamilyName = Info;
		break;
	}
	case PD_Gender:
	{
		DataToChange->Gender = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthMonth:
	{
		DataToChange->BirthMonth = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthDay:
	{
		DataToChange->BirthDay = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BirthYear:
	{
		DataToChange->BirthYear = DBConvert::StringToInt(Info);
		break;
	}
	case PD_BornCountry:
	{
		DataToChange->BirthCountry = Info;
		break;
	}
	case PD_EducationDegree:
	{
		DataToChange->EducationDegree = DBConvert::StringToInt(Info);
		break;
	}
	case PD_ImageFile:
	{
		DataToChange->ImageFile = Info;
		break;
	}
	case PD_WhereLive:
	{
		DataToChange->WhereLive = Info;
		break;
	}
	case PD_MaritalStatus:
	{
		MembersData.MaritalStatus = DBConvert::StringToInt(Info);
		Status					  = MembersData.GetStatus(); // local
		break;
	}
	case PD_ChildrenNum:
	{
		MembersData.ChildrenNum = DBConvert::StringToInt(Info);
		ChildrenNum				= MembersData.ChildrenNum; // local
		break;
	}
	case PD_MailCountry:
	{
		MembersData.MailCountry = Info;
		break;
	}
	case PD_MailCity:
	{
		MembersData.MailCity = Info;
		break;
	}
	case PD_MailStreet:
	{
		MembersData.MailStreet = Info;
		break;
	}
	case PD_MailHomeNumber:
	{
		MembersData.MailHomeNumber = Info;
		break;
	}
	case PD_MailZipCode:
	{
		MembersData.MailZipCode = DBConvert::StringToInt(Info);
		break;
	}
	}
}

void DBWindowWriter::SetEditboxStyle(LONG Style, int TextLimit)
{
	SetWindowLongPtr(WriterEditBox.Window, GWL_STYLE, EDIT_STYLE_BASE | Style);
	SendMessage(WriterEditBox.Window, EM_SETLIMITTEXT, TextLimit, 0);
}

void DBWindowWriter::FinishWriting()
{
	const bool IsParent = PeopleType == EPeopleType::PT_Parent;
	const bool IsSpouse = PeopleType == EPeopleType::PT_Spouse;
	const bool IsChild	= ! (IsParent || IsSpouse);

	if (IsParent)
	{
		Finish = Status != EMeritialStatus::MS_Married && ChildrenNum == EnteredChildrenNum;
	}

	if (IsSpouse)
	{
		Finish = ChildrenNum == EnteredChildrenNum;
	}

	if (IsChild)
	{
		Finish = ChildrenNum == EnteredChildrenNum;
	}

	if (Finish)
	{
		MessageBox(NULL, L"Ну как бы ок", L"Dialog Box", MB_OK);

		if (! GetSystem()) return;

		auto DataManager = GetSystem()->GetComponent<DBDataManager>();
		if (! DataManager) return;

		DataManager->AddMember(MembersData);

		if (MembersData.Parents.size() > 1)
		{
			MembersData.SwitchParents();
			DataManager->AddMember(MembersData);
		}

		OnWriteSuccess.Broadcast();
	}
}
