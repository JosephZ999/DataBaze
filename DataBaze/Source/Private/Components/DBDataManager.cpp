#include "DBDataManager.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include "DBCharCodes.h"

DBDataManager::DBDataManager()
{
	SelectedMemberId.FolderId = 1;
	SelectedMemberId.MemberId = 1;

	using namespace DBPaths;
	CreatePath(GetDataPath());
	SearchValidFolders();

	std::sort(ValidFolders->begin(), ValidFolders->end(),
		[](const int& Value1, const int& Value2) -> bool //
		{ return Value1 < Value2; });
}

void DBDataManager::AddMember(const DBFamilyData& MemberData)
{
	const auto FileName = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	// Read Json file
	if (CheckFile(FileName))
	{
		// Read file data
		std::ifstream File(FileName);
		FileReader.parse(File, FileData);
		File.close();

		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"].append(Family);

		// Save to file
		WriteToDisk(FileName, FileData);
	}
	else
	{
		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"].append(Family);

		// Save to file
		WriteToDisk(FileName, FileData);
	}
	OnMemberAdded.Broadcast();
}

void DBDataManager::SetMember(FMemberId InId, const DBFamilyData& MemberData)
{
	const auto FileName = GenerateJsonPath(InId.FolderId);

	Json::Reader FileReader;
	Json::Value	 FileData;

	// Read Json file
	if (CheckFile(FileName))
	{
		// Read file data
		std::ifstream File(FileName);
		FileReader.parse(File, FileData);
		File.close();

		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"][InId.MemberId] = Family;

		// Save to file
		WriteToDisk(FileName, FileData);
	}
	else
	{
		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"][InId.MemberId] = Family;

		// Save
		WriteToDisk(FileName, FileData);
	}
	OnMemberChanged.Broadcast();
}

bool DBDataManager::LoadMember(DBFamilyData& OutMemberData)
{
	return LoadMemberByIndex(GetSelectedMemberId(), OutMemberData);
}

bool DBDataManager::LoadMemberByIndex(int Id, DBFamilyData& OutMemberData)
{
	const auto FileName = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FileName);
	if (File.good())
	{
		FileReader.parse(File, FileData);
		DeserializeFamily(FileData["Main"][Id], OutMemberData);
		File.close();
		return true;
	}
	return false;
}

bool DBDataManager::SearchValidFolders()
{
	bool SomeFileWasFound = false;
	for (size_t i = 1; i <= MAX_FOLDERS_NUM; ++i)
	{
		const std::wstring FilePath = GenerateJsonPath(i);
		std::ifstream	   File(FilePath);
		if (File.good())
		{
			SomeFileWasFound = true;
			ValidFolders->push_back(i);
			File.close();
		}
		else
		{
			InvalidFolders->push_back(i);
		}
	}
	// if (InvalidFolders->size() > 0)
	{
		for (size_t i = 0; i < ADD_EMPTYFOLDERS_NUM; ++i)
		{
			if (i < InvalidFolders->size())
			{
				int AdditionalFolder = (*InvalidFolders)[i];
				DBPaths::CreatePath(DBPaths::GetDataFolderPath(AdditionalFolder));
				ValidFolders->push_back((*InvalidFolders)[i]);
			}
			else
			{
				break;
			}
		}
		/*
		int AdditionalFolder = (*InvalidFolders)[0];
		DBPaths::CreatePath(DBPaths::GetDataFolderPath(AdditionalFolder));
		ValidFolders->push_back((*InvalidFolders)[0]);
		*/
	}
	/*
	#ifdef _DEBUG
		std::wstring Message;
		Message.append(L"Valid Folders Num : ").append(std::to_wstring(ValidFolders->size()));
		MessageBox(NULL, Message.c_str(), L"Func: SearchValidFolders", MB_OK);

		for (auto& Elem : *ValidFolders)
		{
			std::wstring Message;
			Message.append(L"Valid Folder : ").append(std::to_wstring(Elem));
			MessageBox(NULL, Message.c_str(), L"Func: SearchValidFolders", MB_OK);
		}
	#endif
	*/
	return SomeFileWasFound;
}

int DBDataManager::GetValidFoldersNum()
{
	return ValidFolders->size();
}

bool DBDataManager::CheckFile(const std::wstring& InFilePath)
{
	return std::ifstream(InFilePath).good();
}

void DBDataManager::FillFamilyInfo(const DBFamilyData& MemberData, Json::Value& OutValue)
{
	const bool HasChildren = MemberData.IsHasChildren();
	const bool HasSpouse   = MemberData.IsHasASpouse();

	Json::Value FamilyMember;

	{
		Json::Value Globals;
		std::string Id = JCK_GLOBALS;

		Globals[JGK_LOCK]			= Json::Value(MemberData.bLocked);
		Globals[JGK_STATUS]			= Json::Value(MemberData.MaritalStatus);
		Globals[JGK_CHILDNUM]		= Json::Value(MemberData.ChildrenNum);
		Globals[JGK_MAILCOUNTRY]	= Json::Value(MemberData.MailCountry);
		Globals[JGK_MAILREGION]		= Json::Value(MemberData.MailRegion);
		Globals[JGK_MAILCITY]		= Json::Value(MemberData.MailCity);
		Globals[JGK_MAILSTREET]		= Json::Value(MemberData.MailStreet);
		Globals[JGK_MAILHOMENUMBER] = Json::Value(MemberData.MailHomeNumber);
		Globals[JGK_MAILZIP]		= Json::Value(MemberData.MailZipCode);
		FamilyMember[Id]			= Globals;
	}

	for (int i = 0; i < (HasSpouse ? 2 : 1); ++i)
	{
		Json::Value MemberItem;
		std::string Id = (JCK_PARENT);
		Id.append(std::to_string(i + 1));

		FillPeopleInfo(MemberData.Parents[i], MemberItem);
		FamilyMember[Id] = MemberItem;
	}

	if (HasChildren)
	{
		for (int i = 0; i < (int)MemberData.Children.size(); ++i)
		{
			Json::Value MemberItem;
			std::string Id;
			Id.append(JCK_CHILD).append(std::to_string(i + 1));

			FillPeopleInfo(MemberData.Children[i], MemberItem, true);
			FamilyMember[Id] = MemberItem;
		}
	}

	OutValue = FamilyMember;
}

void DBDataManager::FillPeopleInfo(const DBPeopleData& People, Json::Value& OutValue, bool IsChild)
{
	OutValue[JPK_NAME]		 = Json::Value(People.Name);
	OutValue[JPK_FAMILYNAME] = Json::Value(People.FamilyName);
	OutValue[JPK_GENDER]	 = Json::Value(People.Gender);

	OutValue[JPK_BIRTHMONTH] = Json::Value(People.BirthMonth);
	OutValue[JPK_BIRTHDAY]	 = Json::Value(People.BirthDay);
	OutValue[JPK_BIRTHYEAR]	 = Json::Value(People.BirthYear);

	OutValue[JPK_BIRTHCITY]	   = Json::Value(People.BirthCity);
	OutValue[JPK_BIRTHCOUNTRY] = Json::Value(People.BirthCountry);
	OutValue[JPK_IMAGE]		   = Json::Value(People.ImageFile);

	if (IsChild) return;
	OutValue[JPK_LIVECOUNTRY] = Json::Value(People.WhereLive);
	OutValue[JPK_EDUCATION]	  = Json::Value(People.EducationDegree);
}

void DBDataManager::DeserializeFamily(const Json::Value& InFamily, DBFamilyData& OutFamily)
{
	// Globals
	Json::Value Globals		 = InFamily[JCK_GLOBALS];
	OutFamily.bLocked		 = Globals[JGK_LOCK].asBool();
	OutFamily.MaritalStatus	 = Globals[JGK_STATUS].asInt();
	OutFamily.ChildrenNum	 = Globals[JGK_CHILDNUM].asInt();
	OutFamily.MailCountry	 = Globals[JGK_MAILCOUNTRY].asString();
	OutFamily.MailRegion	 = Globals[JGK_MAILREGION].asString();
	OutFamily.MailCity		 = Globals[JGK_MAILCITY].asString();
	OutFamily.MailStreet	 = Globals[JGK_MAILSTREET].asString();
	OutFamily.MailHomeNumber = Globals[JGK_MAILHOMENUMBER].asString();
	OutFamily.MailZipCode	 = Globals[JGK_MAILZIP].asInt();

	// Parents info
	const bool IsMarried = Globals[JGK_STATUS].asInt() == 2;
	for (int i = 1; i <= (IsMarried ? 2 : 1); ++i)
	{
		std::string Key;
		Key.append(JCK_PARENT).append(std::to_string(i));
		DBPeopleData Parent;
		DeserializePeople(InFamily[Key], Parent);
		OutFamily.Parents.push_back(Parent);
	}

	// Children info
	for (int i = 1; i <= OutFamily.ChildrenNum; ++i)
	{
		std::string Key;
		Key.append(JCK_CHILD).append(std::to_string(i));
		DBPeopleData Child;
		DeserializePeople(InFamily[Key], Child, true);
		OutFamily.Children.push_back(Child);
	}
}

void DBDataManager::DeserializePeople(const Json::Value& InPeople, DBPeopleData& OutPeople, bool IsChild)
{
	OutPeople.Name		 = InPeople[JPK_NAME].asString();
	OutPeople.FamilyName = InPeople[JPK_FAMILYNAME].asString();

	OutPeople.Gender	 = InPeople[JPK_GENDER].asInt();
	OutPeople.BirthMonth = InPeople[JPK_BIRTHMONTH].asInt();
	OutPeople.BirthDay	 = InPeople[JPK_BIRTHDAY].asInt();
	OutPeople.BirthYear	 = InPeople[JPK_BIRTHYEAR].asInt();

	OutPeople.BirthCity	   = InPeople[JPK_BIRTHCITY].asString();
	OutPeople.BirthCountry = InPeople[JPK_BIRTHCOUNTRY].asString();
	OutPeople.ImageFile	   = InPeople[JPK_IMAGE].asString();

	if (IsChild) return;

	OutPeople.WhereLive		  = InPeople[JPK_LIVECOUNTRY].asString();
	OutPeople.EducationDegree = InPeople[JPK_EDUCATION].asInt();
}

int DBDataManager::GetSelectedFolderIndex() const
{
	for (size_t i = 0; i < ValidFolders->size(); ++i)
	{
		if (GetSelectedFolderId() == (*ValidFolders)[i])
		{
			return i;
		}
	}
	return 0;
}

int DBDataManager::ReadImageId()
{
	const auto	  FilePath = GenerateImageDataPath();
	std::ifstream File(FilePath);
	if (File.good())
	{
		std::stringstream buffer;
		buffer << File.rdbuf();
		File.close();
		return DBConvert::StringToInt(buffer.str());
	}
	return 0;
}

void DBDataManager::WriteImageId(int Id)
{
	const auto FilePath = GenerateImageDataPath();
	const auto Prefix	= std::string((Id < 10) ? "00" : ((Id < 100) ? "0" : ""));

	std::ofstream File(FilePath);
	File << std::string(Prefix).append(std::to_string(Id));
	File.close();
}

void DBDataManager::SaveMemberCode(FMemberId InId, const std::wstring& FileName, const std::wstring& Data)
{
	const std::wstring Path = DBPaths::GetConfirmationPath(InId.FolderId).append(FileName).append(L".txt");

	std::wofstream FileStream(Path, std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
	FileStream << Data;
	FileStream.close();

	// backup
	const std::wstring BackupPath = DBPaths::GetBackupConfirmationPath(InId.FolderId).append(FileName);
	std::ifstream	   BackupFile;
	BackupFile.open(std::wstring(BackupPath).append(L".txt"));
	if (BackupFile)
	{
		BackupFile.close();

		time_t	  rawtime;
		struct tm timeinfo;
		wchar_t	  buffer[20];

		time(&rawtime);
		localtime_s(&timeinfo, &rawtime);

		wcsftime(buffer, 20, L"%d_%H-%M-%S", &timeinfo);
		std::wstring Postfix = std::wstring(L"_").append(buffer);

		std::wofstream NewBackupFile(std::wstring(BackupPath).append(Postfix).append(L".txt"), //
			std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		NewBackupFile << Data;
		NewBackupFile.close();
	}
	else
	{
		std::wofstream BackupFile(std::wstring(BackupPath).append(L".txt"), //
			std::ios_base::out | std::ios_base::trunc | std::ios_base::binary);
		BackupFile << Data;
		BackupFile.close();
	}
}

void DBDataManager::LockSelectedItem(bool Lock)
{
	const auto FileName = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FileName);
	if (File.good())
	{
		FileReader.parse(File, FileData);
		FileData["Main"][GetSelectedMemberId()][JCK_GLOBALS][JGK_LOCK] = Json::Value(Lock);
		File.close();
		WriteToDisk(FileName, FileData);
	}
}

void DBDataManager::LockMember(bool InLock, FMemberId InId)
{
	const auto FileName = GenerateJsonPath(InId.FolderId);

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FileName);
	if (File.good())
	{
		FileReader.parse(File, FileData);
		FileData["Main"][InId.MemberId][JCK_GLOBALS][JGK_LOCK] = Json::Value(InLock);
		File.close();
		WriteToDisk(FileName, FileData);
	}
}

std::wstring DBDataManager::GenerateJsonPath() const
{
	return std::wstring(DBPaths::GetDataFolderPath(GetSelectedFolderId()).append(L"\\Data.json"));
}

std::wstring DBDataManager::GenerateJsonPath(int Id) const
{
	return std::wstring(DBPaths::GetDataFolderPath(Id).append(L"\\Data.json"));
}

std::wstring DBDataManager::GenerateImageDataPath() const
{
	return std::wstring(DBPaths::GetDataFolderPath(GetSelectedFolderId())).append(L"\\ImageId.txt");
}

void DBDataManager::GetMembersList(std::vector<std::wstring>& OutList)
{
	auto FilePath = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FilePath);
	FileReader.parse(File, FileData);
	File.close();

	for (size_t i = 0; i < FileData["Main"].size(); ++i)
	{
		OutList.push_back(GetMemberStatus(FileData["Main"], i));
	}
}

bool DBDataManager::GetMemberStatus(int MemberId, std::wstring& OutList)
{
	auto FilePath = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FilePath);
	FileReader.parse(File, FileData);
	File.close();

	if ((int)FileData["Main"].size() > MemberId)
	{
		OutList = GetMemberStatus(FileData["Main"], MemberId);
		return true;
	}
	return false;
}

void DBDataManager::GetLastMemberStatus(std::wstring& OutList)
{
	auto FilePath = GenerateJsonPath();

	Json::Reader FileReader;
	Json::Value	 FileData;

	std::ifstream File(FilePath);
	FileReader.parse(File, FileData);
	File.close();

	OutList = GetMemberStatus(FileData["Main"], FileData["Main"].size() - 1);
}

std::wstring DBDataManager::GetMemberStatus(Json::Value& InData, int InId)
{
	std::string Info;
	std::string ParamId = JCK_PARENT;
	ParamId.append(std::to_string(1));

	Json::Value Member = InData[InId];
	Info.append(Member[ParamId][JPK_NAME].asString()).append(" ");
	Info.append(Member[ParamId][JPK_FAMILYNAME].asString());

	std::wstring Status;
	DBConvert::StringToWString(Info, Status);

	const bool bLocked = Member[JCK_GLOBALS][JGK_LOCK].asBool();

	const int SymbolPosition = 35 - Status.length();
	for (int i = 0; i < SymbolPosition; ++i)
	{
		Status.append(L" ");
	}

	Status.append(bLocked ? (CC_CHECK) : L"");

	return Status;
}

void DBDataManager::SelectMember(FMemberId InMemberId)
{
	SelectedMemberId.MemberId = InMemberId.MemberId;
	SelectedMemberId.ListItem = InMemberId.ListItem;
}

bool DBDataManager::ChangeFolder(bool bNext)
{
	return SetFolder(bNext ? GetSelectedFolderIndex() + 1 : GetSelectedFolderIndex() - 1);
}

bool DBDataManager::SetFolder(int FolderId)
{
	if (ValidFolders->size() == 0) return false;

	const int InitialValue = GetSelectedFolderId();

	if (FolderId >= (int)ValidFolders->size())
	{
		SelectedMemberId.FolderId = (*ValidFolders)[ValidFolders->size() - 1];
		return InitialValue != GetSelectedFolderId();
	}
	else if (FolderId < 0)
	{
		SelectedMemberId.FolderId = (*ValidFolders)[0];
		return InitialValue != GetSelectedFolderId();
	}
	SelectedMemberId.FolderId = (*ValidFolders)[FolderId];
	return InitialValue != GetSelectedFolderId();
}

void DBDataManager::WriteToDisk(const std::wstring& InFileName, const Json::Value& InData)
{
	WriteJsonToDiskBackup(InData);
	WriteJsonToDisk(InFileName, InData);
}

void DBDataManager::WriteJsonToDisk(const std::wstring& InFileName, const Json::Value& InData)
{
	std::ofstream						FileStream(InFileName);
	Json::StreamWriterBuilder			Builder;
	std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
	Writer->write(InData, &FileStream);
	FileStream.close();
}

void DBDataManager::WriteJsonToDiskBackup(const Json::Value& InData)
{
	const std::wstring BackupFileName = std::wstring(DBPaths::GetDataFolderPath(GetSelectedFolderId()).append(L"\\Backup.json"));
	WriteJsonToDisk(BackupFileName, InData);
}
