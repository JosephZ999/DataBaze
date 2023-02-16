#pragma once
#include "DBInterface.h"
#include "DBFunctionLibrary.h"
#include <vector>

#include "json/reader.h"
#include "json/writer.h"

#include "DBDelegate.h"

DECLARE_DELEGATE(OnListChangedSignature);
DECLARE_DELEGATE(OnItemChangedSignature);

#define MAX_FOLDERS_NUM	   20
#define MAX_MEMBERS_NUM	   500

// Json keys

#define JGK_LOCK		   "Lock"
#define JGK_STATUS		   "Status"
#define JGK_CHILDNUM	   "ChildrenNum"
#define JGK_MAILCOUNTRY	   "MailCountry"
#define JGK_MAILCITY	   "MailCity"
#define JGK_MAILSTREET	   "MailStreet"
#define JGK_MAILHOMENUMBER "MailHomeNumber"
#define JGK_MAILZIP		   "MailZipCode"

#define JPK_NAME		   "Name"
#define JPK_FAMILYNAME	   "FamilyName"
#define JPK_GENDER		   "Gender"
#define JPK_BIRTHMONTH	   "BirthM"
#define JPK_BIRTHDAY	   "BirthD"
#define JPK_BIRTHYEAR	   "BirthY"
#define JPK_BIRTHCOUNTRY   "BirthCountry"
#define JPK_LIVECOUNTRY	   "LiveCountry"
#define JPK_IMAGE		   "Image"
#define JPK_EDUCATION	   "EducationDegree"

#define JCK_GLOBALS		   "1.Global"
#define JCK_PARENT		   "2.Parent "
#define JCK_CHILD		   "3.Child "

class DBDataManager : public DBInterface
{
	typedef std::vector<int> Folders;

public:
	DBDataManager();

	OnListChangedSignature OnListChanged;

private:
	int		SelectedFolderId;
	int		SelectedMemberId;
	Folders ValidFolders[MAX_FOLDERS_NUM];

public:
	void SelectMember(int InMemberId);
	bool ChangeFolder(bool bNext);

	bool SearchValidFolders();
	int	 GetValidFoldersNum();
	void LoadFiles();
	void AddMember(const DBFamilyData& MemberData);
	void LoadMember(DBFamilyData& OutMemberData);
	void GetMembersList(std::vector<std::wstring>& OutList);
	int	 GetFolderId() { return SelectedFolderId; }

	std::wstring GenerateImagePath();
	std::wstring GenerateImageName();
private:
	bool SetFolder(int FolderId);

	bool CheckFile(const std::wstring& InFilePath);
	void FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue);
	void FillPeopleInfo(const DBPeopleData& People, Json::Value& OutValue);

	std::wstring GenerateJsonPath(bool CreateFolder = true);
	std::wstring GenerateJsonPath(int Id, bool CreateFolder = true);
	std::wstring GenerateDataPath(int InId, bool CreateFolder = false);
	std::wstring GetMemberStatus(Json::Value& InData, int InId);
};
