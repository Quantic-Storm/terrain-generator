#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>

namespace json
{
	enum class type { _NULL, _BOOL, _STRING, _ARRAY, _OBJECT, _NUMBER };
	struct Value {
	private:
		type contentType;
		void* content;
		

	public:

		Value() { content = nullptr; contentType = type::_NULL; }
		Value(std::vector<Value>& arr) { content = new std::vector<Value>(arr); contentType = type::_ARRAY; }
		Value(std::unordered_map<std::string, Value>& obj) { content = new std::unordered_map<std::string, Value> (obj); contentType = type::_OBJECT; }
		Value(std::string str) { content = new std::string(str), contentType = type::_STRING; }
		Value(bool b) { content = new bool(b), contentType = type::_BOOL; }
		Value(double d) { content = new double(d), contentType = type::_NUMBER; }
		Value(const Value& v);
		
		~Value() { delete content; };

		void setNull() { delete content; content = nullptr; contentType = type::_NULL; }
		void setArray(const std::vector<Value>& vec);
		void setObject(const std::unordered_map<std::string, Value>& obj);
		void setString(const std::string& s);
		void setBool(bool boo);
		void setNumber(double dou);

		Value& operator=(const Value& v);

		bool* getBool();
		double* getDouble();
		bool isNull();
		std::string* getString();
		std::vector<Value>* getArray();
		std::unordered_map<std::string, Value>* getObject();
		const type getType() const { return contentType; }
		std::string to_string();

		void addElement(std::string name, Value element);
		void addElement(Value element);
		
	};
}

