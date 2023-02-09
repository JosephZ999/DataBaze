#include "DBDataManager.h"
#include "json/json.h"
#include "json/writer.h"

#include <iostream>
#include <fstream>

using namespace std;
// using namespace Json;

void DBDataManager::LoadFiles()
{
	const wstring ProjectPath = DBPaths::GetProjectPath();

	// Try to write array as json file

	// Generate json file name
	const wstring FileName = wstring(ProjectPath).append(L"\\ExampleArray.json");

	Json::Reader Reader;
	Json::Value Root;

	string text ="{ \"people\": [{\"id\": 1, \"name\":\"MIKE\",\"surname\":\"TAYLOR\"}, {\"id\": 2, \"name\":\"TOM\",\"surname\":\"JERRY\"} ]}";
	Reader.parse(text, Root);

	Json::StreamWriterBuilder Builder;

	std::unique_ptr<Json::StreamWriter> Writer(Builder.newStreamWriter());

	ofstream File(FileName);
	Writer->write(Root, &File);

	OnUpdate.Broadcast();
}

void DBDataManager::CheckFiles()
{

	const wstring ProjectPath = DBPaths::GetProjectPath();

	for (size_t i = 1; i <= 20; ++i)
	{
		wstring FilePath = ProjectPath;

		char str[256];
		sprintf_s(str, sizeof(str), "\\Folder_%i\\", i);

		FilePath.append(wstring(&str[0], &str[strlen(str)])).append(L"data.json");

		ifstream file(FilePath);

		// json reader
		Json::Reader reader;

		// this will contain complete JSON data
		Json::Value completeJsonData;

		// reader reads the data and stores it in completeJsonData
		if (reader.parse(file, completeJsonData))
		{
			// get value from index
			const auto Data = completeJsonData["name"].asCString();

			// convert value to wstring
			const wstring a(&Data[0], &Data[strlen(Data)]);

			MessageBox(NULL, a.c_str(), L"Dialog Box", MB_OK);
		}
		// MessageBox(NULL, FilePath.c_str(), L"Dialog Box", MB_OK);
	}

	// complete JSON data
	// cout << "Complete JSON data : " << endl << completeJsGrade << endl;

	// get the value associated with name key
	// cout << "Name : " << completeJsonData["name"] << endl;

	// get the value associated with grade key
	// cout << "Grade : " << completeJsonData["grade"] << endl;
}
