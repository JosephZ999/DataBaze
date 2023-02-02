#pragma once

#include <string>
#include <vector>
#include <map>

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
	IDC_NONE = 500,
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

	EDBWinCompId Id		= EDBWinCompId::IDC_NONE;
	HWND		 Window = 0;
	HWND		 Parent = 0;
	Size2D		 Position;
	Size2D		 Size;
	std::wstring Text	  = L"Button";
	int			 FontSize = 16;
	HINSTANCE	 HIns	  = NULL;
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

	std::wstring Name;		   //
	std::wstring FamilyName;   //
	std::wstring BirthMonth;   //
	std::wstring BirthDay;	   //
	std::wstring BirthYear;	   //
	std::wstring BirthCountry; //
	std::wstring WhereLive;	   //

	std::wstring EducationDegree; //
	std::wstring ImageFile;		  //
	std::wstring ChildrenNum;	  //
};

typedef std::vector<DBPeopleData> PeopleGroup;

struct DBFamilyData
{
	DBFamilyData()
		: Parents({})
		, Children({})
		, bFamily(false)
		, bDouble(false)
		, bLocked(false)
	{
	}

	DBFamilyData(PeopleGroup InParents, PeopleGroup InChildren, bool IsFamily)
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

	std::wstring MailCountry; //
	std::wstring MailCity;	  //
	std::wstring MailHome;	  //
	std::wstring MailZipCode; //
};

class DBListContainer
{

public:
	std::vector<DBFamilyData> Items;
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

enum EHotKeyId
{
	HK_None = 0,

	// Viewer
	HKV_Command_1,
	HKV_Command_2,
	HKV_Command_3,
	HKV_Command_4,

	// Writer
	HKW_Enter,
};


// Writer Data

enum EPeopleType
{
	PT_None,
	PT_Parent,
	PT_Spouse,
	PT_Child_1,
	PT_Child_2,
	PT_Child_3,
	PT_Child_4,
	PT_Child_5,
	PT_Child_6,
	PT_Child_7,
	PT_Child_8,
	PT_Child_9,
};

enum EPeopleData
{
	PD_None = 0,
	PD_Name,
	PD_FamilyName,

	PD_BirthMonth,
	PD_BirthDay,
	PD_BirthYear,

	PD_BornCountry,
	PD_EducationDegree,
	PD_ImageFile,

	PD_NotChildInfo,

	PD_WhereLive,

	PD_OnlyParentInfo,

	PD_ChildrenNum,
	PD_MailCountry,
	PD_MailCity,
	PD_MailHome,
	PD_MailZipCode,
	PD_Max,
};
