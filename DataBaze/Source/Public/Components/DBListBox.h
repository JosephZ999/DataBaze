#pragma once
#include "DBInterface.h"
#include <string>

class DBListBox : public DBInterface
{
	// properties
public:
private:
	int LastItemId = 0;

	// functions
public:
	void Initialize(const std::vector<std::wstring>& InList) {}
	void AddItem(const std::wstring& InItem) {}

private:
};
