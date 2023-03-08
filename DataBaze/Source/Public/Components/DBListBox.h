#pragma once
#include "DBInterface.h"
#include "DBDataTypes.h"
#include <string>

class DBListBox : public DBInterface
{
public:
private:
	int	   LastItemId  = 0;
	HWND   ListBoxHWND = nullptr;
	Size2D InitialPos;
	Size2D InitialSize;

	// functions
public:
	void Initialize(EDBWinCompId Id, DWORD Style, const Size2D& Position, const Size2D& Size, HWND OwnerWnd, int FontSize);
	void SetList(const std::vector<std::wstring>& InList);
	void AddItem(const std::wstring& InItem);
	void ClearList();

	int GetSelectedItemId();

	inline int GetLastItemId() const;

	Size2D GetInitialPos() { return InitialPos; }
	Size2D GetInitialSize() { return InitialSize; }

	void SetSize(const Size2D& InSize);
	void SetPosition(const Size2D& InPos);
	HWND GetWnd() const { return ListBoxHWND; }

private:
};
