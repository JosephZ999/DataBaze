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

#define JGK_MAILCOUNTRY	   "1.MailCountry"
#define JGK_MAILREGION	   "1.MailRegion"
#define JGK_MAILCITY	   "2.MailCity"
#define JGK_MAILSTREET	   "3.MailStreet"
#define JGK_MAILHOMENUMBER "4.MailHomeNumber"
#define JGK_MAILZIP		   "5.MailZipCode"
#define JGK_STATUS		   "6.Status"
#define JGK_CHILDNUM	   "7.ChildrenNum"
#define JGK_LOCK		   "8.Lock"

#define JPK_NAME		   "1.Name"
#define JPK_FAMILYNAME	   "2.FamilyName"
#define JPK_GENDER		   "3.Gender"
#define JPK_BIRTHMONTH	   "4.BirthM"
#define JPK_BIRTHDAY	   "5.BirthD"
#define JPK_BIRTHYEAR	   "6.BirthY"
#define JPK_BIRTHCOUNTRY   "7.BirthCountry"
#define JPK_LIVECOUNTRY	   "8.LiveCountry"
#define JPK_EDUCATION	   "9.EducationDegree"
#define JPK_IMAGE		   "Image"

#define JCK_GLOBALS		   "1.Global"
#define JCK_PARENT		   "2.Parent "
#define JCK_CHILD		   "3.Child "

class DBDataManager : public DBInterface
{
	typedef std::vector<int> Folders;

public:
	DBDataManager();

	OnListChangedSignature OnMemberAdded;
	OnItemChangedSignature OnMemberChanged;

private:
	int		SelectedFolderId;
	int		SelectedMemberId;
	Folders ValidFolders[MAX_FOLDERS_NUM];
	Folders InvalidFolders[MAX_FOLDERS_NUM];

public:
	void SelectMember(int InMemberId);
	bool ChangeFolder(bool bNext);

	bool SearchValidFolders();
	int	 GetValidFoldersNum();

	void AddMember(const DBFamilyData& MemberData);
	void SetMember(int MemberId, int FolderId, const DBFamilyData& MemberData);

	bool LoadMember(DBFamilyData& OutMemberData);
	bool LoadMemberByIndex(int Id, DBFamilyData& OutMemberData);
	int	 GetSelectedMemberId() { return SelectedMemberId; }

	void GetMembersList(std::vector<std::wstring>& OutList);
	void GetLastMemberStatus(std::wstring& OutList);

	int GetFolderId() const { return SelectedFolderId; }
	int GetSelectedFolderIndex() const;

	int	 ReadImageId();
	void WriteImageId(int Id);

	void SaveMemberCode(int MemberId, int FolderId, const std::wstring& FileName, const std::wstring& Data);

private:
	bool SetFolder(int FolderId);
	void WriteToDisk(const std::wstring& InFileName, const Json::Value& InData);
	void WriteJsonToDisk(const std::wstring& InFileName, const Json::Value& InData);
	void WriteJsonToDiskBackup(const Json::Value& InData);

	bool CheckFile(const std::wstring& InFilePath);
	void FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue);
	void FillPeopleInfo(const DBPeopleData& People, Json::Value& OutValue, bool IsChild = false);
	void DeserializeFamily(const Json::Value& InFamily, DBFamilyData& OutFamily);
	void DeserializePeople(const Json::Value& InPeople, DBPeopleData& OutPeople, bool IsChild = false);

	std::wstring GenerateJsonPath() const;
	std::wstring GenerateJsonPath(int Id) const;
	std::wstring GenerateImageDataPath() const;
	std::wstring GetMemberStatus(Json::Value& InData, int InId);
};
