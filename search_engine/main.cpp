#include "search_engine.h"
#include <iostream>

using namespace std;

/**
* TESTS
*/
TEST(sample_test_case, sample_test)
{
	EXPECT_EQ(1, 1);
}
/*void TestInvertedIndexFunctionality(
	const vector<string>& docs,
	const vector<string>& requests,
	const vector<vector<Entry>>& expected)
{
	vector<vector<Entry>> result;
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	for (auto& request : requests) {
		vector<Entry> word_count = idx.GetWordCount(request);
		result.push_back(word_count);
	} 
	ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic)
{
	const vector<string> docs = {
		"london is the capital of great britain",
		"big ben is the nickname for the Great bell of the striking clock"
	};
	const vector<string> requests = { "london", "the" };
	const vector<vector<Entry>> expected = {
		{{0, 1}},
		{{0, 1}, {1, 3}}
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2)
{
	const vector<string> docs = {
		"milk milk milk milk water water water",
		"milk water water",
		"milk milk milk milk milk water water water water water",
		"americano cappuccino"
	};
	const vector<string> requests = { "milk", "water", "cappuchino" };
	const vector<vector<Entry>> expected = {
		{{0, 4}, {1, 1}, {2, 5}},
		{{0, 2}, {1, 2}, {2, 5}},
		{{3, 1}}
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
} 
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord)
{
	const vector<string> docs = {
		"a b c d e f g h i j k l",
		"statement"
	};
	const vector<string> requests = { "m", "statement" };
	const vector<vector<Entry>> expected = {
		{ }, {{1, 1}}
	};
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseSearchServer, TestSimple)
{
	const vector<string> docs = {
		"milk milk milk milk water water water",
		"milk water water",
		"milk milk milk milk milk water water water water water",
		"americano cappuccino"
	};
	const vector<string> request = { "milk water", "sugar" };
	const vector<vector<RelativeIndex>> expected = {
		{{2, 1}, {0, 0.7}, {1, 0.3}},
		{ }
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	vector<vector<RelativeIndex>> result = srv.search(request);
	ASSERT_EQ(result, expected);
} 
TEST(TestCaseSearchServer, TestTop5)
{
	const vector<string> docs = {
		"london is the capital of great britain",
		"paris is the capital of france",
		"berlin is the capital of germany",
		"rome is the capital of italy",
		"madrid is the capital of spain",
		"lisboa is the capital of portugal",
		"bern is the capital of switzerland",
		"moscow is the capital of russia",
		"kiev is the capital of ukraine",
		"minsk is the capital of belarus",
		"astana is the capital of kazakhstan",
		"beijing is the capital of china",
		"tokyo is the capital of japan",
		"bangkok is the capital of thailand",
		"welcome to moscow the capital of russia the third rome",
		"amsterdam is the capital of netherlands",
		"helsinki is the capital of finland",
		"oslo is the capital of norway","stockholm is the capital of sweden",
		"riga is the capital of latvia",
		"tallinn is the capital of estonia",
		"warsaw is the capital of poland",
	};
	const vector<string> request = { "moscow is the capital of russia" };
	const vector<vector<RelativeIndex>> expected = {
		{
		{7, 1},	{14, 1}, {0, 0.666666687},
		{1, 0.666666687}, {2, 0.666666687}
		}
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	vector<vector<RelativeIndex>> result = srv.search(request);
	ASSERT_EQ(result, expected);
}*/

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
						//cout << "Letters: " << numberLetter - 1 << endl;
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
	ofstream fileAnswer("../../../../search_engine/answers.json");
	nlohmann::json dictRequest;
	nlohmann::json dict;
	dict = { "answers", dictRequest };
	fileAnswer << dict;
	if (fileAnswer.bad() != true)
		cout << "File answers.json is full\n";
}

//Exceptions
const char* FileOpenException::what() const noexcept
{
	return "Config file is missing!\n";
}

const char* FileEmptyException::what() const noexcept
{
	return "Config file is empty!\n";
}

void InvertedIndex::UpdateDocumentBase(vector<string> input_docs)
{
	
	docs = input_docs;
	size_t docSize = docs.size();
	struct Entry entry[5];	//Разобраться с размером, должен быть docs.size()
	for (int i = 0; i < docs.size(); i++)
	{
		entry[i].doc_id = i;
		cout << docs[i] << endl;
	}
}

//Main
int main()
{
	ConverterJSON* converter = new ConverterJSON();
	InvertedIndex* inverted = new InvertedIndex();

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
	ofstream fileAnswer("../../../../search_engine/answers.json", ios::out);
	vector<vector<pair<int, float>>> answers;
	converter->putAnswers(answers);


	return 0;
}
