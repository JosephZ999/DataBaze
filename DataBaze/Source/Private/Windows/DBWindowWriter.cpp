
#include "DBWindowWriter.h"
#include "DBFunctionLibrary.h"

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

		DBLib::CreateStaticBox(WriterInfoBox, WS_VISIBLE | WS_CHILD);
	}
	case WM_COMMAND:
	{
		if (wParam == WM_SHOWWINDOW)
		{
			if (WriterObj) break; // something is wrong

			WriterObj = new DBWindowWriter(hWnd);
			SetFocus(WriterEditBox.Window);

			WriterObj->SelectWriteData(WriterObj->PeopleType);
			break;
		}

		break;
	}
	case WM_PAINT:
	{
		// DBLib::CreateText(hWnd);
	}
	case WM_KEYDOWN:
	{
		// MessageBox(NULL, L"Sorry. It does't work", L"Dialog Box", MB_OK);
		break;
	}
	case WM_CLOSE:
	{
		DBLib::SetWindowVisibility(hWnd, false);
		if (WriterObj)
		{
			delete WriterObj;
			WriterObj = nullptr;
		}
		return 0;
	}
	case WM_HOTKEY:
	{
		if (! WriterObj) break;

		WriterObj->WriteData();
		WriterObj->SelectWriteData(WriterObj->PeopleType);
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
			RegisterHotKey(hWnd, 5, 0, VK_RETURN);
		}
		if (HIWORD(wParam) == EN_KILLFOCUS)
		{
			UnregisterHotKey(hWnd, 5);
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

void DBWindowWriter::SelectWriteData(EPeopleType PT)
{
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
}

void DBWindowWriter::WriteData()
{
	PeopleData = static_cast<EPeopleData>(PeopleData + 1);
	if (PeopleType >= PT_Child_1 && PeopleData >= PD_NotChildrenInfo)
	{
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
	case PD_BirthMonth:			InfoText.append(L"Birth Month");				break;
	case PD_BirthDay:			InfoText.append(L"Birth Day");					break;
	case PD_BirthYear:			InfoText.append(L"Birth Year");					break;
	case PD_BornCountry:		InfoText.append(L"Country where born");			break;
	case PD_WhereLive:			InfoText.append(L"Country where live today");	break;
	case PD_MailCountry:		InfoText.append(L"Mailing country");			break;
	case PD_MailCity:			InfoText.append(L"Mailing city");				break;
	case PD_MailHome:			InfoText.append(L"Mailing home number");		break;
	case PD_MailZipCode:		InfoText.append(L"Mailing ZipCode");			break;
	case PD_EducationDegree:	InfoText.append(L"Education");					break;
	case PD_ChildrenNum:		InfoText.append(L"Education");					break;
	} // clang-format on

	SetInfoText(InfoText);
}

void DBWindowWriter::SelectChild(int Index)
{
	while (MembersData.Children.size() < Index)
	{
		MembersData.Parents.push_back(DBPeopleData());
	}
	DataToChange = &MembersData.Children[Index - 1];
}

void DBWindowWriter::SetInfoText(std::wstring& Text)
{
	SendMessage(WriterInfoBox.Window, WM_SETTEXT, 0, (LPARAM)Text.c_str());
}
