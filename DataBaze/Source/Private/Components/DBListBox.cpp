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

	SetWindowPos(ListBoxHWND, HWND_TOP, Position.X, Position.Y, Size.X, Size.Y, 0);

	HFONT hFont = CreateFont(FontSize, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
		CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("consolas"));
	SendMessage(ListBoxHWND, WM_SETFONT, (WPARAM)hFont, 0);

	// DBLib::SetFontSize(ListBoxHWND, FontSize);
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
	SetWindowPos(ListBoxHWND, HWND_TOP, 0, 0, InSize.X, InSize.Y, SWP_NOMOVE);
}

void DBListBox::SetPosition(const Size2D& InPos)
{
	SetWindowPos(ListBoxHWND, HWND_TOP, InPos.X, InPos.Y, 0, 0, SWP_NOSIZE);
}

void DBListBox::SetSelectedItemText(const std::wstring& InText)
{
	const int Index = (int)SendMessage(ListBoxHWND, LB_GETCURSEL, 0, 0);
	const int Data	= (int)SendMessage(ListBoxHWND, LB_GETITEMDATA, Index, 0);

	// del
	SendMessage(ListBoxHWND, LB_DELETESTRING, Index, 0);

	// create
	int ItemId = SendMessage(ListBoxHWND, LB_ADDSTRING, 0, (LPARAM)InText.c_str());
	SendMessage(ListBoxHWND, LB_SETITEMDATA, ItemId, Data);

	// scroll
	SendMessage(ListBoxHWND, LB_SETCURSEL, ItemId, 0);
}
