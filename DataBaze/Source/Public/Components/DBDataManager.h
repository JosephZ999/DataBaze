#pragma once
#include "DBInterface.h"
#include "DBFunctionLibrary.h"
#include <vector>

#include "json/reader.h"
#include "json/writer.h"

#include "DBDelegate.h"

DECLARE_DELEGATE(OnListChangedSignature);
DECLARE_DELEGATE(OnItemChangedSignature);

#define MAX_FOLDERS_NUM	   50
#define MAX_MEMBERS_NUM	   300

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
#define JPK_BIRTHCITY	   "7.BirthCity"
#define JPK_BIRTHCOUNTRY   "8.BirthCountry"
#define JPK_LIVECOUNTRY	   "9.LiveCountry"
#define JPK_EDUCATION	   "EducationDegree"
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
	FMemberId SelectedMemberId;
	Folders	  ValidFolders[MAX_FOLDERS_NUM];
	Folders	  InvalidFolders[MAX_FOLDERS_NUM];

public:
	/* Foler id unused */
	void SelectMember(FMemberId InMemberId);
	bool ChangeFolder(bool bNext);

	bool SearchValidFolders();
	int	 GetValidFoldersNum();

	void AddMember(const DBFamilyData& MemberData);
	void SetMember(FMemberId InId, const DBFamilyData& MemberData);

	bool LoadMember(DBFamilyData& OutMemberData);
	bool LoadMemberByIndex(int Id, DBFamilyData& OutMemberData);

	void GetMembersList(std::vector<std::wstring>& OutList);
	bool GetMemberStatus(int MemberId, std::wstring& OutList);
	void GetLastMemberStatus(std::wstring& OutList);

	int		  GetSelectedMemberId() { return SelectedMemberId.MemberId; }
	int		  GetSelectedFolderId() const { return SelectedMemberId.FolderId; }
	FMemberId GetMemberId() const { return SelectedMemberId; }

	int GetSelectedFolderIndex() const;

	int	 ReadImageId();
	void WriteImageId(int Id);

	void SaveMemberCode(FMemberId InId, const std::wstring& FileName, const std::wstring& Data);
	void LockSelectedItem(bool Lock);
	void LockMember(bool InLock, FMemberId InId);

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
