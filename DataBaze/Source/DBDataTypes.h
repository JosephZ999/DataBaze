#pragma once

#include <string>
#include <vector>

struct Size2D
{
	Size2D() {}
	Size2D(int InX, int InY)
		: X(InX)
		, Y(InY)
	{
	}

	int X = 0;
	int Y = 0;

	Size2D& operator=(const Size2D& NewSize);
};

inline Size2D& Size2D::operator=(const Size2D& NewSize)
{
	X = NewSize.X;
	Y = NewSize.Y;
	return *this;
}

enum DBButtonId
{
	IDB_NONE = 10,
	IDB_DIALOG,
	IDB_NEWITEM,
	IDB_LOCK,
	IDB_UNLOCK,
	// IDB_EXIT
};

struct DBButtonBase
{
	DBButtonBase() {}
	DBButtonBase(DBButtonId InId, HWND InWindow, Size2D InPosition, Size2D InSize, std::wstring InText)
		: Id(InId)
		, Window(InWindow)
		, Position(InPosition)
		, Size(InSize)
		, Text(InText)
	{
	}

	DBButtonId	 Id		= DBButtonId::IDB_NONE;
	HWND		 Window = 0;
	Size2D		 Position;
	Size2D		 Size;
	std::wstring Text;
};

class ButtonContainer
{
	DBButtonBase Buttons[10];
	int			 ButtonLastIndex = 0;

public:
	inline void Add(const DBButtonBase& InButton);
	inline bool FindByIndex(DBButtonId Id, DBButtonBase& Button);
	bool IsEmpty() { return ButtonLastIndex == 0; }
};

struct DBPeopleData
{
	DBPeopleData() {}
	DBPeopleData(std::wstring InName)
		: Name(InName)

	{
	}

	std::wstring StateInFamily; // Parent 1, Child 1
	std::wstring Name;			//
	std::wstring FamilyName;	//
	std::wstring BirthData;		// mm/dd/yyyy
	std::wstring Info;			//
};

typedef std::vector<DBPeopleData> PeopleGroup;

struct DBListItem
{
	DBListItem() {}
	DBListItem(PeopleGroup InParents, PeopleGroup InChildren, bool IsFamily)
		: Parents(InParents)
		, Children(InChildren)
		, bFamily(IsFamily)
	{
	}

	PeopleGroup Parents;
	PeopleGroup Children;

	bool bFamily;
	bool bDouble;
	bool bLocked;
};

class DBListContainer
{

public:
	std::vector<DBListItem> Items;
};

// container implementations

void ButtonContainer::Add(const DBButtonBase& InButton)
{
	if (ButtonLastIndex < 10)
	{
		Buttons[ButtonLastIndex] = InButton;
		++ButtonLastIndex;
		return;
	}
}

bool ButtonContainer::FindByIndex(DBButtonId Id, DBButtonBase& Button)
{
	for (int i = 0; i < 10; ++i)
	{
		if (Buttons[i].Id != Id) continue;

		Button = Buttons[i];
		return true;
	}

	return false;
}
