#pragma once

#include <string>
#include <vector>
#include <map>

#include "Size2D.h"
#include "framework.h"

enum EDBWindowType
{
	WT_None,
	WT_Static,
	WT_Button,
	WT_Edit,
	WT_ComboBox,
	WT_ListBox,
};

enum EDBWinCompId
{
	// Main window comps
	IDC_NONE = 400,
	IDC_VIEW,
	IDC_NEWITEM,
	IDC_LOCK,
	IDC_UNLOCK,
	IDC_ListBox,

	IDC_PrevFolder,
	IDC_NextFolder,
	IDC_Minimize,
	IDC_FolderId,

	IDC_ListLength,

	// Viewer window comps
	IDC_V_Info,
	IDC_V_InfoTitle,

	// btns
	IDC_V_Edit,
	IDC_V_Prev,
	IDC_V_Next,

	// Writer window comps
	IDC_W_Info,
	IDC_W_MoreInfo,
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
	PD_Gender,

	PD_BirthMonth,
	PD_BirthDay,
	PD_BirthYear,

	PD_BornCity,
	PD_BornCountry,
	PD_ImageFile,

	PD_NotChildInfo,

	PD_EducationDegree,
	PD_WhereLive,

	PD_OnlyParentInfo,

	PD_MaritalStatus,
	PD_ChildrenNum,

	PD_MailCountry,
	PD_MailRegion,
	PD_MailCity,
	PD_MailStreet,
	PD_MailHomeNumber,
	PD_MailZipCode,
	PD_Max,
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
	{}

	EDBWinCompId Id		= EDBWinCompId::IDC_NONE;
	HWND		 Window = 0;
	HWND		 Parent = 0;
	Size2D		 Position;
	Size2D		 Size;
	std::wstring Text	  = L"Button";
	int			 FontSize = 16;
	HINSTANCE	 HIns	  = NULL;
};

struct FWndItem
{
	FWndItem() {}
	FWndItem(EDBWindowType InType, EDBWinCompId InId, Size2D InSize, std::wstring InText)
		: Type(InType)
		, Id(InId)
		, Size(InSize)
		, Text(InText)
	{}

	FWndItem(EDBWindowType InType, EDBWinCompId InId, std::wstring InText)
		: Type(InType)
		, Id(InId)
		, Text(InText)
		, AddStyle(BS_NOTIFY)
	{}

	FWndItem(EDBWindowType InType, EDBWinCompId InId, Size2D InSize, std::wstring InText, int InFontSize, DWORD InStyle)
		: Type(InType)
		, Id(InId)
		, Size(InSize)
		, Text(InText)
		, FontSize(InFontSize)
		, AddStyle(InStyle)
	{}

	EDBWindowType Type = EDBWindowType::WT_None;
	EDBWinCompId  Id   = EDBWinCompId::IDC_NONE;
	Size2D		  Position;
	Size2D		  Size;
	std::wstring  Text	   = L"Text";
	int			  FontSize = 16;
	DWORD		  AddStyle = 0;
};

class FWindowContainer
{
	int LastIndex = 0;

public:
	DBWindow Windows[10];

	void Add(const DBWindow& InButton);
	bool FindByIndex(EDBWinCompId Id, DBWindow& OutWindow) const;
	bool Contains(EDBWinCompId Id) const;
	bool IsEmpty() { return LastIndex == 0; }
};

enum EEducationDegree
{
	ED_HighSchoolNoDegree = 1,
	ED_HighSchoolDegree,
	ED_vocationalSchool,
	ED_SomeUniversityCourses,
	ED_UniversityDegree,
	ED_SomeGraduatelevelCourses,
	ED_MastersDegree,
	ED_SomeDoctoralCourses,
	ED_Doctorate
};

struct DBPeopleData
{
	DBPeopleData() {}
	DBPeopleData(std::string InName)
		: Name(InName)
	{}

	std::string Name;		//
	std::string FamilyName; //

	int Gender	   = 0; // 1 Mele and 2 Femele
	int BirthMonth = 0; //
	int BirthDay   = 0; //
	int BirthYear  = 0; //

	std::string BirthCity;			 //
	std::string BirthCountry;		 //
	std::string WhereLive;			 //
	std::string ImageFile;			 //
	int			EducationDegree = 0; //

	bool IsMele() const { return Gender == 1; }
	bool IsBirthCityValid() const;

	EEducationDegree GetEducation() const;
	std::string		 GetBirthDataAsString() const;
	std::string		 GetGenderAsString() const;
	std::string		 GetEducationAsString() const;
	std::string		 GetAsString(EPeopleData DataType);
};

enum EMeritialStatus
{
	MS_Unmarried = 1,
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
		, bLocked(false)
	{}

	DBFamilyData(PeopleGroup InParents, PeopleGroup InChildren, bool IsFamily)
		: Parents(InParents)
		, Children(InChildren)
		, bLocked(false)
	{}

	PeopleGroup Parents;
	PeopleGroup Children;

	bool bLocked = false;

	int MaritalStatus = 0; // use enum EMeritialStatus
	int ChildrenNum	  = 0; //

	std::string MailCountry;	 //
	std::string MailRegion;		 //
	std::string MailCity;		 //
	std::string MailStreet;		 //
	std::string MailHomeNumber;	 //
	int			MailZipCode = 0; //

	bool IsHasChildren() const;
	bool IsHasASpouse() const;
	bool IsHasZipCode() const;

	EMeritialStatus GetStatus() const;

	void SwitchParents();

	DBPeopleData& operator[](EPeopleType Type);
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
	HKV_Next,
	HKV_Reset,

	// Writer
	HKW_Enter,
};

struct FMemberId
{
	int MemberId = 0;
	int FolderId = 1;
	int ListItem = 0;
};
