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

enum EDBButtonId
{
	IDB_NONE = 10,
	IDB_VIEW,
	IDB_NEWITEM,
	IDB_LOCK,
	IDB_UNLOCK,
	IDB_MAX,
};

enum EWindows
{
	IDW_NONE = EDBButtonId::IDB_MAX,
	IDW_VIEWER,
	IDW_WRITER,
	IDW_MAX,
};

struct DBButton
{
	DBButton() {}
	DBButton(EDBButtonId InId, HWND InWindow, Size2D InPosition, Size2D InSize, std::wstring InText)
		: Id(InId)
		, Window(InWindow)
		, Position(InPosition)
		, Size(InSize)
		, Text(InText)
	{
	}

	EDBButtonId	 Id		= EDBButtonId::IDB_NONE;
	HWND		 Window = 0;
	HWND		 Parent = 0;
	Size2D		 Position;
	Size2D		 Size;
	std::wstring Text	  = L"Button";
	int			 FontSize = 16;
};

class ButtonContainer
{
	DBButton Buttons[10];
	int		 ButtonLastIndex = 0;

public:
	inline void Add(const DBButton& InButton);
	inline bool FindByIndex(EDBButtonId Id, DBButton& Button);
	bool		IsEmpty() { return ButtonLastIndex == 0; }
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
	DBListItem()
		: Parents({})
		, Children({})
		, bFamily(false)
		, bDouble(false)
		, bLocked(false)
	{
	}

	DBListItem(PeopleGroup InParents, PeopleGroup InChildren, bool IsFamily)
		: Parents(InParents)
		, Children(InChildren)
		, bFamily(IsFamily)
		, bDouble(false)
		, bLocked(false)
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

void ButtonContainer::Add(const DBButton& InButton)
{
	if (ButtonLastIndex < 10)
	{
		Buttons[ButtonLastIndex] = InButton;
		++ButtonLastIndex;
		return;
	}
}

bool ButtonContainer::FindByIndex(EDBButtonId Id, DBButton& Button)
{
	for (int i = 0; i < 10; ++i)
	{
		if (Buttons[i].Id != Id) continue;

		Button = Buttons[i];
		return true;
	}

	return false;
}
