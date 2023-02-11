#pragma once

#include <string>
#include <vector>
#include <map>

#include "Size2D.h"
#include "framework.h"

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

	void Add(const DBWindow& InButton);
	bool FindByIndex(EDBWinCompId Id, DBWindow& Button);
	bool IsEmpty() { return ButtonLastIndex == 0; }
};

struct DBPeopleData
{
	DBPeopleData() {}
	DBPeopleData(std::wstring InName)
		: Name(InName)
	{
	}

	int			 Id;
	std::wstring Name;		 //
	std::wstring FamilyName; //

	int BirthMonth; //
	int BirthDay;	//
	int BirthYear;	//

	std::wstring BirthCountry;	  //
	std::wstring WhereLive;		  //
	std::wstring EducationDegree; //
	std::wstring ImageFile;		  //

	std::wstring ToWString();
};

enum EMeritialStatus
{
	MS_Unmarried,
	MS_Married,
	MS_USMarried,
	MS_Divorced,
	MS_Widowed,
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

	EMeritialStatus MaritalStatus; // use enum EMeritialStatus
	std::wstring	MailCountry;   //
	std::wstring	MailCity;	   //
	std::wstring	MailHome;	   //
	std::wstring	MailZipCode;   //

	std::wstring ToWString();

	int	 GetChildrenNum() const;
	bool IsHasChildren() const;
	bool IsHasASpouse() const;
};

class DBListContainer
{

public:
	std::vector<DBFamilyData> Items;
};

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

	PD_MaritalStatus,
	PD_ChildrenNum,
	PD_MailCountry,
	PD_MailCity,
	PD_MailHome,
	PD_MailZipCode,
	PD_Max,
};
