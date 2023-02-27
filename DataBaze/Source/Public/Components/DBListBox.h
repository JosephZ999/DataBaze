#pragma once
#include "DBInterface.h"
#include "DBDataTypes.h"
#include <string>

class DBListBox : public DBInterface
{
public:

private:
	int	 LastItemId	 = 0;
	HWND ListBoxHWND = nullptr;

	// functions
public:
	void Initialize(EDBWinCompId Id, DWORD Style, const Size2D& Position, const Size2D& Size, HWND OwnerWnd, int FontSize);
	void SetList(const std::vector<std::wstring>& InList);
	void AddItem(const std::wstring& InItem);
	void ClearList();

	int GetSelectedItemId();

	inline int GetLastItemId() const;

private:
};
