#include "FileParser.h"


using namespace json;

FileParser::FileParser(FileParser& fp) {
	filename = fp.filename;
	file = std::fstream(filename, std::fstream::in);	// the reading
	content = fp.content;
}

FileParser::FileParser(std::string fn) {
	filename = fn;
	file = std::fstream(filename, std::fstream::in);
}

bool FileParser::isNumberChar(char c) {
	std::string chars = "0123456789eE+-.";
	return (chars.find(c) != std::string::npos);
}

void FileParser::parse() {
	char ch;
	try {
		while (file >> std::noskipws >> ch) {
			if (!isWhiteSpace(ch)) {
				if (ch == '{') content.setObject(parseObject());
			}
		}
	}
	catch (std::exception* ex) {
		ex->what();
		delete ex;
	}
	
}


std::unordered_map<std::string, Value> FileParser::parseObject() {
	char ch;
	unsigned int state = 0;
	std::unordered_map<std::string, Value> obj;
	std::string name;
	Value val;
	while (file >> std::noskipws >> ch) {
		if (!isWhiteSpace(ch)) {

			switch (state) 
			{
			case 0 :
				if (ch == '}') return obj;
				else if (ch == '"')
				{
					name = parseString();
					state++;
				}
				else throw new std::exception("parsing error, expecting token \" or }");
				break;

			case 1 :
				if (ch == ':') state++;
				else throw new std::exception("parsing error, expecting token :");
				break;

			case 2 :
				if (isNumberChar(ch)) {
					val.setNumber(parseNumber(&ch));
					state++;
				}
				else if (ch == '"') {
					state++;
					val.setString(parseString());
				}
				else if (ch == 't') {
					state++;
					(parseTrue()) ? val.setBool(true) : throw new std::exception("parsing error while parsing token true");
				}
				else if (ch == 'f') {
					state++;
					(parseFalse()) ? val.setBool(false) : throw new std::exception("parsing error while parsing token false");
				}
				else if (ch == 'n') {
					state++;
					(parseNull()) ? val.setNull() : throw new std::exception("parsing error while parsing token null");
				}
				else if (ch == '[') {
					state++;
					val.setArray(parseArray());
				}
				else if (ch == '{') {
					state++;
					val.setObject(parseObject());
				}
				else throw new std::exception("parsing error, unknown token %c", ch);
				break;

			case 3 :
				if (ch == ',') {
					obj[name] = val;
					state = 0;
				}
				else if (ch == '}') {
					obj[name] = val;
					return obj;
				}
				else throw new std::exception("parsing error, expecting token , or }");
				break;

			default: state = 0;
			}
		}
	}
}


void FileParser::writeToFile(std::string fn) {
	std::fstream output(fn, std::fstream::out);

	output << content.to_string();

	output.close();
}

bool FileParser::parseTrue() {
	unsigned int i;
	char ch;
	std::string str;
	for (i = 0; i < 3; i++) {
		file >> std::noskipws >> ch;
		str += ch;
	}
	return (str == std::string("rue"));
}

bool FileParser::parseFalse() {
	unsigned int i;
	char ch;
	std::string str;
	for (i = 0; i < 4; i++) {
		file >> std::noskipws >> ch;
		str += ch;
	}
	return (str == std::string("alse"));
}

bool FileParser::parseNull() {
	unsigned int i;
	char ch;
	std::string str;
	for (i = 0; i < 3; i++) {
		file >> std::noskipws >> ch;
		str += ch;
	}
	return (str == std::string("ull"));
}

std::string FileParser::parseString() {
	char ch;
	std::string res;
	while (file >> std::noskipws >> ch) {
		if (ch == '"') return res;
		else if (ch == '\\') {
			file >> std::noskipws >> ch;
			switch (ch) {
			case '"':
			case '/':
			case '\\':
				res.push_back(ch);
				break;
			case 'b':
				res.push_back('\b');
				break;
			case 'f':
				res.push_back('\f');
				break;
			case 'n':
				res.push_back('\n');
				break;
			case 'r':
				res.push_back('\r');
				break;
			case 't':
				res.push_back('\t');
				break;
			default :
				throw new std::exception("parsing error : \\%c unknown", ch);
			}
		}
		else res.push_back(ch);
	}
}

double FileParser::parseNumber(char* ch) {
	std::string numString;
	numString.push_back(*ch);
	while (file >> std::noskipws >> *ch) {
		if (isNumberChar(*ch)) numString.push_back(*ch);
		else return std::stod(numString);
	}
}

std::vector<Value> FileParser::parseArray() {
	char ch;
	unsigned int state = 0;
	std::vector<Value> res;
	while (file >> std::noskipws >> ch) {
		if (!isWhiteSpace(ch)) {
			if (state == 0) {
				Value val;
				if (ch == ']') return res;
				else if (ch == '{') {
					val.setObject(parseObject());
					res.push_back(val);
					state++;
				}
				else if (ch == '[') {
					val.setArray(parseArray());
					res.push_back(val);
					state++;
				}
				else if (ch == '"') {
					val.setString(parseString());
					res.push_back(val);
					state++;
				}
				else if (isNumberChar(ch)) {
					val.setNumber(parseNumber(&ch));
					res.push_back(val);
					state++;
				}
				else if (ch == 't') {
					(parseTrue()) ? val.setBool(true) : throw new std::exception("parsing error while parsing token true");
					res.push_back(val);
					state++;
				}
				else if (ch == 'f') {
					(parseFalse()) ? val.setBool(false) : throw new std::exception("parsing errorwhile parsing token false");
					res.push_back(val);
					state++;
				}
				else if (ch == 'n') {
					(parseNull()) ? val.setNull() : throw new std::exception("parsing error while parsing token null");
					res.push_back(val);
					state++;
				}
				else throw new std::exception("parsing error, unknown token %c", ch);
			}
			if (state == 1) {
				if (ch == ']') return res;
				else if (ch == ',') state = 0;
				else throw new std::exception("parsing error, expecting , or ]");
			}
		}
	}
}


