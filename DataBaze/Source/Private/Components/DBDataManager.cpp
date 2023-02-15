#include "DBDataManager.h"
#include <iostream>
#include <fstream>

DBDataManager::DBDataManager()
{
	SelectedFolderId = 1;
	SelectedMemberId = 1;
}

void DBDataManager::LoadFiles()
{
	const std::wstring FileName = GenerateJsonPath();

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
		const std::wstring FilePath = GenerateJsonPath(i, false);
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

			FillPeopleInfo(MemberData.Children[i], MemberItem);
			FamilyMember[Id] = MemberItem;
		}
	}

	OutValue = FamilyMember;
}

void DBDataManager::FillPeopleInfo(const DBPeopleData& People, Json::Value& OutValue)
{
	OutValue[JPK_NAME]		 = Json::Value(People.Name);
	OutValue[JPK_FAMILYNAME] = Json::Value(People.FamilyName);
	OutValue[JPK_GENDER]	 = Json::Value(People.Gender);

	OutValue[JPK_BIRTHMONTH] = Json::Value(People.BirthMonth);
	OutValue[JPK_BIRTHDAY]	 = Json::Value(People.BirthDay);
	OutValue[JPK_BIRTHYEAR]	 = Json::Value(People.BirthYear);

	OutValue[JPK_BIRTHCOUNTRY] = Json::Value(People.BirthCountry);
	OutValue[JPK_LIVECOUNTRY]  = Json::Value(People.WhereLive);
	OutValue[JPK_IMAGE]		   = Json::Value(People.ImageFile);
	OutValue[JPK_EDUCATION]	   = Json::Value(People.EducationDegree);
}

std::wstring DBDataManager::GenerateImagePath()
{
	return GenerateDataPath(SelectedFolderId, true);
}

std::wstring DBDataManager::GenerateImageName()
{
	std::wstring FilePath = GenerateDataPath(SelectedFolderId, true);
	FilePath.append(L"\\NextImage.txt");
	std::string ImageName = "001";

	std::ifstream ifFile(FilePath);
	if (ifFile.good())
	{
		std::stringstream buffer;
		buffer << ifFile.rdbuf();
		ImageName = buffer.str();
		ifFile.close();

		int ImageId = DBConvert::StringToInt(ImageName);
		++ImageId;

		std::string prefix = (ImageId < 10) ? "00" : ((ImageId < 100) ? "0": "");

		std::ofstream ofFile(FilePath);
		ofFile.clear();
		ofFile << prefix << ImageId;
		ofFile.close();

		std::wstring OutValue;
		DBConvert::StringToWString(ImageName, OutValue);
		return std::wstring().append(L"\\").append(OutValue).append(L".jpg");
	}
	
	std::ofstream ofFile(FilePath);
	ofFile << "002";
	ofFile.close();
	return std::wstring(L"\\001.jpg");
}

std::wstring DBDataManager::GenerateJsonPath(bool CreateFolder)
{
	return GenerateDataPath(SelectedFolderId, CreateFolder).append(L"\\Data.json");
}

std::wstring DBDataManager::GenerateJsonPath(int Id, bool CreateFolder)
{
	return GenerateDataPath(Id, CreateFolder).append(L"\\Data.json");
}

std::wstring DBDataManager::GenerateDataPath(int InId, bool CreateFolder)
{
	auto Folder = DBPaths::GetProjectPath().append(L"\\Data");
	_wmkdir(Folder.c_str());

	Folder.append(L"\\Folder_").append(std::to_wstring(InId));
	if (CreateFolder)
	{
		_wmkdir(Folder.c_str());
	}
	return Folder;
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

std::wstring DBDataManager::GetMemberStatus(Json::Value& InData, int InId)
{
	std::string Info;
	std::string ParamId = JCK_PARENT;
	ParamId.append(std::to_string(1));
	Info.append(InData[InId][ParamId][JPK_NAME].asString()).append(" ");
	Info.append(InData[InId][ParamId][JPK_FAMILYNAME].asString());

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
