#pragma once
#include "Value.h"

namespace json
{
	class FileWriter {
	private :
		std::string name;
		json::Value val;

	public :
		FileWriter();
		FileWriter(std::string filename);
		~FileWriter() {}

		std::string& getName() { return name; }
		void setName(std::string newName) { name = newName; }
		
		json::Value& getValue() { return val; }

		void writeFile();

	};
}
