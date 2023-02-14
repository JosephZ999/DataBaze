#include "DBDataManager.h"
#include <iostream>
#include <fstream>

void DBDataManager::LoadFiles()
{
	const std::wstring FileName = GenerateFileLocation();

	DBFamilyData Family;
	DBFamilyData Family2;
	DBFamilyData Family3;

	DBPeopleData People;
	People.Name		  = "Joseph";
	People.FamilyName = "Zzz";
	Family.Parents.push_back(People);

	People.Name		  = "Di";
	People.FamilyName = "Moon";
	Family2.Parents.push_back(People);

	People.Name		  = "DN";
	People.FamilyName = "Age";
	Family3.Parents.push_back(People);

	AddMember(Family);
	AddMember(Family2);
	AddMember(Family3);
}

void DBDataManager::AddMember(const DBFamilyData& MemberData)
{
	const auto FileName = GenerateFileLocation();

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
		std::ofstream FileStream(FileName);

		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
		Writer->write(FileData, &FileStream);
		FileStream.close();
	}
	else
	{
		std::ofstream FileStream(FileName);

		// Add New Member
		Json::Value Family;
		FillFamilyInfo(MemberData, Family);
		FileData["Main"].append(Family);

		Json::StreamWriterBuilder			Builder;
		std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());
		Writer->write(FileData, &FileStream);
		FileStream.close();
	}
}

void DBDataManager::LoadMember(DBFamilyData& OutMemberData)
{
	DBFamilyData Member;
	DBPeopleData Parent;
	Parent.Name		  = "Joseph";
	Parent.FamilyName = "Zzz";

	Member.Parents.push_back(Parent);
	OutMemberData = Member;
}

bool DBDataManager::SearchValidFolders()
{
	bool SomeFileWasFound = false;
	for (size_t i = 1; i <= MAX_FOLDERS_NUM; ++i)
	{
		const std::wstring FilePath = GenerateFileLocationById(i);
		std::ifstream	   File(FilePath);
		if (File.good())
		{
			SomeFileWasFound = true;
			ValidFolders->push_back(i);
		}
		File.close();
	}

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
	return SomeFileWasFound;
}

int DBDataManager::GetValidFoldersNum()
{
	return 0;
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
	Json::Value MemberItem;

	MemberItem["Name"] = Json::Value(MemberData.Parents[0].Name);

	MemberItem["FamilyName"] = Json::Value(MemberData.Parents[0].FamilyName);

	FamilyMember["Member 1"] = MemberItem;
	FamilyMember["Member 2"] = MemberItem;
	OutValue				 = FamilyMember;
}

std::wstring DBDataManager::GenerateFileLocation(bool CreateFolder)
{
	return GenerateFileLocationById(SelectedFolderId, CreateFolder);
}

std::wstring DBDataManager::GenerateFileLocationById(int InId, bool CreateFolder)
{
	auto Folder = DBPaths::GetProjectPath().append(L"\\Data");
	_wmkdir(Folder.c_str());

	Folder.append(L"\\Folder_").append(std::to_wstring(InId));
	if (CreateFolder)
	{
		_wmkdir(Folder.c_str());
	}
	return Folder.append(L"\\data.json");
}

void DBDataManager::GetMembersList(std::vector<std::wstring>& OutList)
{
	auto FilePath = GenerateFileLocation();

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

std::wstring DBDataManager::GetMemberStatus(Json::Value& InData, int InId)
{
	std::string Info;
	Info.append(InData[InId]["Member 1"]["Name"].asString()).append(" ");
	Info.append(InData[InId]["Member 1"]["FamilyName"].asString());

	std::wstring Status;
	DBConvert::StringToWString(Info, Status);
	return Status;
}

void DBDataManager::SelectMember(int InMemberId)
{
	SelectedMemberId = InMemberId;
}

bool DBDataManager::ChangeFolder(bool bNext)
{
	return SetFolder(bNext ? SelectedFolderId + 1 : SelectedFolderId - 1);
}

bool DBDataManager::SetFolder(int FolderId)
{
	if (ValidFolders->size() == 0) return false;

	const int InitialValue = SelectedFolderId;

	if (FolderId >= (int)ValidFolders->size())
	{
		SelectedFolderId = ValidFolders->size() - 1;
		return InitialValue != SelectedFolderId;
	}
	else if (FolderId < 0)
	{
		SelectedFolderId = 0;
		return InitialValue != SelectedFolderId;
	}
	SelectedFolderId = FolderId;
	return InitialValue != SelectedFolderId;
}
