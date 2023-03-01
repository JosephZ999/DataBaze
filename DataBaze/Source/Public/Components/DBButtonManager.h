#pragma once

#include "DBInterface.h"
#include "DBDataTypes.h"
#include "DBFunctionLibrary.h"
#include "Components/UIVerticalBox.h"
#include <map>

class DBButtonManager : public DBInterface
{
	typedef std::shared_ptr<UISlot>		 SlotPtr;
	typedef std::vector<SlotPtr>		 UIElems;
	typedef std::map<EDBWinCompId, HWND> UIHandlers;

public:
	~DBButtonManager();

private:
	UIElems	   UISlots;
	UIHandlers Handlers;

public:
	HWND AddItem(HWND OwnerWnd, const FWndItem& Item);
	void AddSlot(SlotPtr Slot);
	HWND GetWndHandler(EDBWinCompId Id);

private:
};
