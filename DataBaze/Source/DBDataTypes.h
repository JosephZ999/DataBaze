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

enum EDBWindowCompType
{
	WCT_None,
	WCT_Static,
	WCT_Button,
	WCT_Edit,
	WCT_ComboBox,
};

enum EDBWinCompId
{
	// Main window comps
	IDC_NONE = 10,
	IDC_VIEW,
	IDC_NEWITEM,
	IDC_LOCK,
	IDC_UNLOCK,

	// Viewer window comps

	// Writer window comps
	IDC_W_Info,
	IDC_W_Edit,
	
	IDC_MAX,
};

enum EWindows
{
	IDW_NONE = EDBWinCompId::IDC_MAX,
	IDW_VIEWER,
	IDW_WRITER,
	IDW_MAX,
};

struct DBWindow
{
	DBWindow() {}
	DBWindow(EDBWinCompId InId, HWND InParent, Size2D InPosition, Size2D InSize, std::wstring InText)
		: Id(InId)
		, Parent(InParent)
		, Position(InPosition)
		, Size(InSize)
		, Text(InText)
	{
	}

	EDBWinCompId	 Id		= EDBWinCompId::IDC_NONE;
	HWND		 Window = 0;
	HWND		 Parent = 0;
	Size2D		 Position;
	Size2D		 Size;
	std::wstring Text	  = L"Button";
	int			 FontSize = 16;
};

class ButtonContainer
{
	int ButtonLastIndex = 0;

public:
	DBWindow Buttons[10];

	inline void Add(const DBWindow& InButton);
	inline bool FindByIndex(EDBWinCompId Id, DBWindow& Button);
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

void ButtonContainer::Add(const DBWindow& InButton)
{
	if (ButtonLastIndex < 10)
	{
		Buttons[ButtonLastIndex] = InButton;
		++ButtonLastIndex;
		return;
	}
}

bool ButtonContainer::FindByIndex(EDBWinCompId Id, DBWindow& Button)
{
	for (int i = 0; i < 10; ++i)
	{
		if (Buttons[i].Id != Id) continue;

		Button = Buttons[i];
		return true;
	}

	return false;
}
