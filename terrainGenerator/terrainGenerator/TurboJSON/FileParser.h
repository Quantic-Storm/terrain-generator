#pragma once
#include "Value.h"


namespace json
{
	class Value;
	class FileParser 
	{
	private :
		std::string filename;
		std::fstream file;
		Value content;

		bool isWhiteSpace(char c) { return (c=='\t' || c==' ' || c=='\n' || c=='\r'); }
		bool isNumberChar(char c);

		std::unordered_map<std::string, Value> parseObject();
		std::string parseString();
		double parseNumber(char* ch);
		bool parseTrue();
		bool parseFalse();
		bool parseNull();
		std::vector<Value> parseArray();
	public :
		FileParser() {}
		FileParser(FileParser& fp);
		FileParser(std::string fn);
		~FileParser() {}

		void parse();
		void writeToFile(std::string fn);
		Value& getParsed() { return content; }
	};
}

