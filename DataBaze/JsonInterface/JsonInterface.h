#pragma once
#include <iostream>
#include <fstream>
#include "json/json.h"

// using namespace std;
// // read file
// ifstream file(“data.json”);
// // json reader
// Json::Reader reader;
// // this will contain complete JSON data
// Json::Value completeJsonData;
// // reader reads the data and stores it in completeJsonData
// reader.parse(file, completeJsonData);
// // complete JSON data
// cout << “Complete JSON data: “ << endl << completeJsGrade << endl;
// // get the value associated with name key
// cout << “Name: “ << completeJsonData[“name”] << endl;
// // get the value associated with grade key
// cout << “Grade: “ << completeJsonData[“grade”] << endl;

// - - -

// Json::Value event;
// Json::Value vec(Json::arrayValue);
// vec.append(Json::Value(1));
// vec.append(Json::Value(2));
// vec.append(Json::Value(3));
//
// event["competitors"]["home"]["name"] = "Liverpool";
// event["competitors"]["away"]["code"] = 89223;
// event["competitors"]["away"]["name"] = "Aston Villa";
// event["competitors"]["away"]["code"]=vec;
// std::cout << event << std::endl;

// - - -

//#include <iostream>
//#include <fstream>
//
// void main()
//{
//	std::ofstream file_id;
//	op_file_id.open("file.txt");
//
//	Json::Value value_obj;
//	//populate 'value_obj' with the objects, arrays etc.
//
//	Json::StyledWriter styledWriter;
//	file_id << styledWriter.write(value_obj);
//
//	file_id.close();
//}

class JsonInterface
{
};
