/**
* Reading configuration data from JSON	   / Считывание конфигурационных данных из JSON
* JSON Request Transformation			   / Преобразование запросов в формате JSON
* Forming responses in a given JSON format / Формирование ответов в заданном формате JSON
*/

#pragma once

#include <iostream>
#include "nlohmann_json/include/nlohmann/json.hpp"
#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <map>
#include <exception>
#include <fstream>

using namespace std;

/**
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

struct Entry
{
	/**
	* doc_id - номер элемента в векторе docs, по которому хранится текст
	* count - число, обозначающее сколько раз ключевое слово встретилось в документе doc_id
	*/
	
	size_t doc_id, count;

	bool operator == (const Entry& other) const
	{
		return (doc_id == other.doc_id && count == other.count);
	}
};


//Хранение и индексирование слов
class InvertedIndex
{
	vector<string> docs; //коллекция для хранения текстов документов
	map<string, vector<Entry>> freq_dictionary; //коллекция для хранения частоты слов, встречаемых в тексте
public:
	InvertedIndex() = default;
	/**
	* Обновить или заполнить базу документов, по которой будем совершать поиск
	* @param input_docs содержимое документов
	*/
	void UpdateDocumentBase(vector<string> input_docs);
	/**
	* Метод определяет количество вхождений слова word в загруженной базе документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает готовый списов с частотой слов 
	*/
	
	vector<Entry> GetWordCount(const string& word);
};

struct RelativeIndex
{
	size_t doc_id;
	float rank;

	bool operator == (const RelativeIndex& other) const
	{
		return (doc_id == other.doc_id && rank == other.rank);
	}
};

class SearchServer
{
	InvertedIndex _index;
public:
	/**
	* @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
	*	чтобы SearchServer мог узнать частоту слов встречаемых в запросе
	*/
	SearchServer(InvertedIndex& idx) : _index(idx) {};
	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы, взятые из файла requests.json
	* @return возвращает отсортированный список релевантных ответов для заданных запросов
	*/
	vector<vector<RelativeIndex>> search(const vector<string>& queries_input);
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