#include "DBButtonManager.h"
#include "Components/UISpacer.h"
#include "UIDataTypes.h"

void DBButtonManager::Initialize(HWND OwnerWnd, std::vector<FWndItem>& Buttons) {}

HWND DBButtonManager::AddItem(HWND OwnerWnd, const FWndItem& Item)
{
	DWORD Style = WS_VISIBLE | WS_CHILD | Item.AddStyle;
	HWND  Wnd	= DBLib::CreateWindowComponent(Item.Type, Item.Text, Style, Item.Position, Item.Size, OwnerWnd, HMENU(Item.Id));
	DBLib::SetFontSize(Wnd, Item.FontSize);
	Handlers[Item.Id] = Wnd;
	return Wnd;
}

void DBButtonManager::AddSlot(UISlot* Slot)
{
	UISlots.push_back(Slot);
	Slot->ReDraw();
}

HWND DBButtonManager::GetWndHandler(EDBWinCompId Id)
{
	return Handlers[Id];
}
