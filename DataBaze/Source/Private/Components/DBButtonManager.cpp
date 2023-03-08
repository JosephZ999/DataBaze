#include "DBButtonManager.h"
#include "Components/UISpacer.h"
#include "UIDataTypes.h"

DBButtonManager::~DBButtonManager()
{
	auto it = Handlers.begin();
	while (it != Handlers.end())
	{
		DestroyWindow(it->second);
		++it;
	}
}

HWND DBButtonManager::AddItem(HWND OwnerWnd, const FWndItem& Item)
{
	DWORD Style = WS_VISIBLE | WS_CHILD | Item.AddStyle;
	HWND  Wnd	= DBLib::CreateWindowComponent(Item.Type, Item.Text, Style, Item.Position, Item.Size, OwnerWnd, HMENU(Item.Id));
	DBLib::SetFontSize(Wnd, Item.FontSize);
	Handlers[Item.Id] = Wnd;
	return Wnd;
}

void DBButtonManager::AddSlot(int SlotId, SlotPtr Slot)
{
	UISlots.insert({SlotId, Slot});
	Slot->ReDraw();
}

DBButtonManager::SlotPtr DBButtonManager::GetSlot(int SlotId)
{
	return UISlots[SlotId];
}

HWND DBButtonManager::GetWndHandler(EDBWinCompId Id)
{
	return Handlers[Id];
}
