#pragma once

#include "DBInterface.h"
#include "DBDataTypes.h"
#include "DBFunctionLibrary.h"
#include "Components/UIVerticalBox.h"

class DBButtonManager : public DBInterface
{
	typedef std::vector<UISlot*> UIElems;
	typedef std::vector<HWND>	 UIHandlers;

public:
private:
	UIElems UISlots;
	// UIHandlers SlotHandlers;
	// functions
public:
	void Initialize(HWND OwnerWnd, std::vector<FWndItem>& Buttons);
	HWND AddItem(HWND OwnerWnd, const FWndItem& Item);
	void AddSlot(UISlot* Slot);

private:
};
