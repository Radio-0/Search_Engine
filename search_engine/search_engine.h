/*
* Reading configuration data from JSON	   / Считывание конфигурационных данных из JSON
* JSON Request Transformation			   / Преобразование запросов в формате JSON
* Forming responses in a given JSON format / Формирование ответов в заданном формате JSON
*/

#pragma once

#include <iostream>
#include "nlohmann_json/include/nlohmann/json.hpp"

#include <vector>
#include <string>
#include <map>
#include <exception>
#include <fstream>

using namespace std;

/*
* The ConvertJSON class for working with JSON files contains:
* Method for reading the max_responses field to determine the limit
* File content retrieval method
* Method for receiving requests from the requeste.json file
* Put search results in answers.json file
*/
class ConverterJSON
{
public:
	ConverterJSON() = default;

	int GetResponsesLimit(map<string, string> field);

	vector<string> GetTextDocuments(vector<string> field);

	vector<string> GetRequests();

	void putAnswers(vector<vector<pair<int, float>>> answers);
};

//Exceptions
class FileOpenException : public exception
{
public:
	const char* what() const noexcept override;
};

class FileEmptyException : public exception
{
public:
	const char* what() const noexcept override;
};