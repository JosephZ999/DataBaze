#include "DBListBox.h"
#include "DBFunctionLibrary.h"

DBListBox::DBListBox()
{
	const DWORD		Style	 = WS_VISIBLE | WS_CHILD | (LBS_NOTIFY | LBS_SORT | WS_VSCROLL);
	const HINSTANCE HIns	 = DBSysLib::GetHIns();
	const Size2D	Position = {200, 25};
	const Size2D	Size	 = {500, 300};

	ListBoxHWND = CreateWindow(L"LISTBOX", //
		L"MemberList",					   //
		Style,							   //
		Position.X, Position.Y,			   //
		Size.X, Size.Y,					   //
		DBSysLib::GetMainHWND(),		   // Parent
		(HMENU)IDC_ListBox,				   // Id
		HIns, NULL);

	DBLib::SetFontSize(ListBoxHWND, 20);
}

void DBListBox::Initialize(const std::vector<std::wstring>& InList)
{
	ClearList();
	for (auto& Elem : InList)
	{
		int ItemId = SendMessage(ListBoxHWND, LB_ADDSTRING, 0, (LPARAM)Elem.c_str());
		SendMessage(ListBoxHWND, LB_SETITEMDATA, ItemId, GetLastItemId());
		++LastItemId;
	}
}

void DBListBox::AddItem(const std::wstring& InItem)
{
	int ItemId = SendMessage(ListBoxHWND, LB_ADDSTRING, 0, (LPARAM)InItem.c_str());
	SendMessage(ListBoxHWND, LB_SETITEMDATA, ItemId, GetLastItemId());
	++LastItemId;
	
}

void DBListBox::ClearList()
{
	SendMessage(ListBoxHWND, LB_RESETCONTENT, 0, 0);
	LastItemId = 0;
}

int DBListBox::GetLastItemId() const
{
	return LastItemId;
}
