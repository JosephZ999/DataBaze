#include "DBListBox.h"
#include "DBFunctionLibrary.h"

void DBListBox::Initialize(EDBWinCompId Id, DWORD Style, const Size2D& Position, const Size2D& Size, HWND OwnerWnd, int FontSize)
{
	InitialPos	= Position;
	InitialSize = Size;

	ListBoxHWND = CreateWindow(L"LISTBOX", //
		L"MemberList",					   //
		Style,							   //
		Position.X, Position.Y,			   //
		Size.X, Size.Y,					   //
		OwnerWnd,						   // Parent
		(HMENU)Id,						   // Id
		NULL, NULL);

	DBLib::SetFontSize(ListBoxHWND, FontSize);
	SetWindowPos(ListBoxHWND, HWND_TOP, Position.X, Position.Y, Size.X, Size.Y, 0);
}

void DBListBox::SetList(const std::vector<std::wstring>& InList)
{
	ClearList();
	for (auto& Elem : InList)
	{
		AddItem(Elem);
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

int DBListBox::GetSelectedItemId()
{
	int Index = (int)SendMessage(ListBoxHWND, LB_GETCURSEL, 0, 0);
	return (int)SendMessage(ListBoxHWND, LB_GETITEMDATA, Index, 0);
}

int DBListBox::GetLastItemId() const
{
	return LastItemId;
}

void DBListBox::SetSize(const Size2D& InSize)
{
}

void DBListBox::SetPosition(const Size2D & InPos)
{
}
