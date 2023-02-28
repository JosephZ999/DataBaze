#pragma once

#include "DBInterface.h"
#include "DBDataTypes.h"
#include "DBFunctionLibrary.h"
#include "Components/UIVerticalBox.h"
#include <map>

class DBButtonManager : public DBInterface
{
	typedef std::vector<UISlot*>		 UIElems;
	typedef std::map<EDBWinCompId, HWND> UIHandlers;

public:
private:
	UIElems	   UISlots;
	UIHandlers Handlers;

public:
	void Initialize(HWND OwnerWnd, std::vector<FWndItem>& Buttons);
	HWND AddItem(HWND OwnerWnd, const FWndItem& Item);
	void AddSlot(UISlot* Slot);

	HWND GetWndHandler(EDBWinCompId Id);
private:
};
