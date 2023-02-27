#include "DBButtonManager.h"

void DBButtonManager::Initialize(HWND OwnerWnd, std::vector<FButton>& Buttons)
{
	for (auto& Elem : Buttons)
	{
		DBLib::CreateButton(OwnerWnd, Elem);
	}
}
