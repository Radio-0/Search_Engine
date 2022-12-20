#include "search_engine.h"
#include <iostream>

using namespace std;

/*
* The ConvertJSON class for working with JSON files
*/
//Max responses
int ConverterJSON::GetResponsesLimit(map<string, string> field)
{
	int count = 0;
	const auto foundKey = field.find("max_responses");
	if (foundKey != field.end())
		count = stoi(foundKey->second);
	else count = 5;
	return count;
}
//Text documents
vector<string>ConverterJSON::GetTextDocuments(vector<string> field)
{
	vector<string> fileContentList;
	string temp;
	string tempWord;
	int i, strIndex, numberWord, numberLetter;
	int fSpace, sSpace;

	for (i = 0; i < field.size(); i++)
	{
		ifstream file(field[i]);
		if (!file.is_open())
			cout << "File not found\n";
		else
		{
			numberWord = 0, fSpace = 0, sSpace = 0, numberLetter = 0;
			while (getline(file, temp))
			{
				for (strIndex = 0; strIndex < temp.length(); strIndex++)
				{
					if (temp[strIndex] == ' ' || temp[strIndex + 1] == '\0')
					{
						fSpace = strIndex;
						numberLetter = fSpace - sSpace;
						if (numberLetter > 1 && numberLetter - 1 <= 100)
							++numberWord;
						else if (numberLetter - 1 > 100)
						{
							cout << "Error number letters" << endl;
							break;
						}
						sSpace = fSpace;
						cout << "Letters: " << numberLetter - 1 << endl;
					}
					if (numberWord > 1000 || numberLetter - 1 > 100)
					{
						//cout << "Error number words" << endl;
						//cout << "Error number letters" << endl;
						break;
					}
				}
				if (numberWord > 1000 || numberLetter - 1 > 100)
				{
					//cout << "Error number words" << endl;
					//cout << "Error number letters" << endl;
					break;
				}
			}
			//cout << "Word: " << numberWord << endl;
			if (numberWord <= 1000 && numberLetter - 1 <= 100)
			{
				fileContentList.push_back(temp);
			}
		}
		file.close();
	}
	return fileContentList;
}
//Get requests
vector<string> ConverterJSON::GetRequests()
{
	ifstream fileRequest("../../../../search_engine/requests.json");
	nlohmann::json dictRequest;
	vector<string> fieldRequests;

	if (!fileRequest.is_open())
		cout << "Request file is empty" << endl;
	else
	{
		fileRequest >> dictRequest;
		fileRequest.close();
	}
	fieldRequests = dictRequest["requests"];
	return fieldRequests;
}
//Put search result
void ConverterJSON::putAnswers(vector<vector<pair<int, float>>> answers)
{

}

//Exceptions
class FileOpenException : public exception
{
public:
	const char* what() const noexcept override
	{
		return "Config file is missing!\n";
	}
};

class FileEmptyException : public exception
{
public:
	const char* what() const noexcept override
	{
		return "Config file is empty!\n";
	}
};

//Main
int main()
{
	ConverterJSON* converter = new ConverterJSON();

	ifstream fileConfig("../../../../search_engine/config.json");
	nlohmann::json dictConfig;

	map<string, string> fieldConfig;
	vector<string> fieldFiles;
	vector<string> pathFiles;
	int max_responses = 0;

	vector<string> fieldRequests;

//Config File
	try
	{
		if (!fileConfig.is_open())
			throw FileOpenException();
		else
		{
			fileConfig >> dictConfig;
			try
			{
				if(dictConfig.find("config") == dictConfig.end())
					throw FileEmptyException();
				else
				{
					fieldConfig = dictConfig["config"];
					const auto foundKey = fieldConfig.find("name");
					cout << "Starting <<" << foundKey->second << ">>" << endl;
				}
			}
			catch (const FileEmptyException& e)
			{
				cerr << e.what();
			}
			fileConfig.close();
		}
	}
	catch (const FileOpenException& e)
	{
		cerr << e.what();
	}
	
	max_responses = converter->GetResponsesLimit(fieldConfig);
	cout << "Max responses: " << max_responses << endl;

	fieldFiles = dictConfig["files"];
	pathFiles = converter->GetTextDocuments(fieldFiles);
	for(int i = 0; i < pathFiles.size(); i++)
		cout << "FILE" << i << ": " << pathFiles[i] << endl;

//Request File
	fieldRequests = converter->GetRequests();
	for(int i = 0; i < fieldRequests.size(); i++)
		cout << "REQUEST" << i << ": " << fieldRequests[i] << endl;

//Answers File
	converter->putAnswers();

	return 0;
}
