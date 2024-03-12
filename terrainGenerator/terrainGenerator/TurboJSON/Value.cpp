#include "Value.h"
#include <iostream>


using namespace json;

std::string Value::to_string() {
	std::string res;
	switch (contentType) {
	case type::_NULL:
		res = "null";
		break;
	case type::_BOOL:
		res = (*(bool*)content) ? "true" : "false";
		break;
	case type::_NUMBER:
		res = std::to_string(*(double*)content);
		break;
	case type::_STRING:
		res = "\"" + * (std::string*)content + "\"";
		break;
	case type::_ARRAY:
		res = "[";
		for (Value v : *(std::vector<Value>*) content) {
			res += v.to_string() + ",";
		}
		res.pop_back();
		res += "]";
		break;
	case type::_OBJECT:
		res = "{";
		for (std::pair<std::string, Value> p : *(std::unordered_map<std::string,Value>*)content) {
			res += "\"" + p.first + "\"" + ":" + p.second.to_string() + "\n";
		}
		res += "}";
		break;
	}
	return res;
}

void Value::setArray(const std::vector<Value>& arr) {
	delete content;
	content = new std::vector<Value>(arr);
	contentType = type::_ARRAY;
}

void Value::setObject(const std::unordered_map<std::string, Value>& obj) {
	delete content;
	content = new std::unordered_map<std::string, Value>(obj);
	contentType = type::_OBJECT;
}

void Value::setString(const std::string& str) {
	delete content;
	content = new std::string(str);
	contentType = type::_STRING;
}

void Value::setNumber(double d) {
	delete content;
	content = new double(d);
	contentType = type::_NUMBER;
}

void Value::setBool(bool b) {
	delete content;
	content = new bool(b);
	contentType = type::_BOOL;
}

bool Value::isNull() {
	return (contentType == type::_NULL);
}

bool* Value::getBool() {
	if (contentType == type::_BOOL) return ((bool*)content);
	else throw new std::exception("incompatible return type");
}

double* Value::getDouble() {
	if (contentType == type::_NUMBER) return ((double*)content);
	else throw new std::exception("incompatible return type");
}

std::string* Value::getString() {
	if (contentType == type::_STRING) return ((std::string*)content);
	else throw new std::exception("incompatible return type");
}

std::vector<Value>* Value::getArray() {
	if (contentType == type::_ARRAY) return ((std::vector<Value>*)content);
	else throw new std::exception("incompatible return type");
}

std::unordered_map<std::string, Value>* Value::getObject() {
	if (contentType == type::_OBJECT) return ((std::unordered_map<std::string, Value>*)content);
	else throw new std::exception("incompatible return type");
}

void Value::addElement(Value element) {
	if (contentType != type::_ARRAY) throw new std::exception("inserting element in wrong type");
	((std::vector<Value>*) content)->push_back(element);
}

void Value::addElement(std::string name, Value element) {
	if (contentType != type::_OBJECT) throw new std::exception("inserting element in wrong type");
	(*((std::unordered_map < std::string, Value > *) content))[name] = element;
}

Value::Value(const Value& v) {
	contentType = v.contentType;
	content = nullptr;
	switch (contentType) {
	case type::_ARRAY :
		content = new std::vector<Value>(*(std::vector<Value>*)(v.content));
		break;
	case type::_BOOL:
		content = new bool(*(bool*)(v.content));
		break;
	case type::_OBJECT:
		content = new std::unordered_map<std::string,Value>(*(std::unordered_map<std::string,Value>*)(v.content));
		break;
	case type::_NULL:
		content = nullptr;
		break;
	case type::_STRING:
		content = new std::string(*(std::string*)(v.content));
		break;
	case type::_NUMBER:
		content = new double(*(double*)(v.content));
		break;
	}
}

Value& Value::operator=(const Value& v) {
	contentType = v.contentType;
	switch (contentType) {
	case type::_ARRAY:
		content = new std::vector<Value>(*(std::vector<Value>*)(v.content));
		break;
	case type::_BOOL:
		content = new bool(*(bool*)(v.content));
		break;
	case type::_OBJECT:
		content = new std::unordered_map<std::string, Value>(*(std::unordered_map<std::string, Value>*)(v.content));
		break;
	case type::_NULL:
		content = nullptr;
		break;
	case type::_STRING:
		content = new std::string(*(std::string*)(v.content));
		break;
	case type::_NUMBER:
		content = new double(*(double*)(v.content));
		break;
	}
	return *this;
}


