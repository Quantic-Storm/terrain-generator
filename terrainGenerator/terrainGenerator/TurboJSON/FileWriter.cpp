#include "FileWriter.h"
#include <fstream>

using namespace json;

FileWriter::FileWriter() : FileWriter("null") {}

FileWriter::FileWriter(std::string filename) {
	name = filename;
	val = Value();
	std::unordered_map<std::string, Value> obj;
	val.setObject(obj);
}

void FileWriter::writeFile() {
	std::ofstream out(name + ".json");
	out << val.to_string();
	out.close();
}