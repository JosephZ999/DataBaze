#pragma once
#include "DBInterface.h"
#include "DBDataTypes.h"
#include <string>

class DBListBox : public DBInterface
{
public:
	DBListBox();

private:
	int	 LastItemId	 = 0;
	HWND ListBoxHWND = nullptr;

	// functions
public:
	void Initialize(const std::vector<std::wstring>& InList);
	void AddItem(const std::wstring& InItem);
	void ClearList();

	inline int GetLastItemId() const;

private:
};
