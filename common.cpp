#include "common.h"


//Parses the parameter seperated by a delimiter delim in a given line, return as a pair of variable name and value
std::pair<std::string, std::string> parseParam(std::string line, char delim) {
	std::string name;
	std::string value;

	size_t pos = line.find(delim);
	if (pos == std::string::npos) {
		throw std::runtime_error(std::format("Tried to parse a line, it did not contain the delimiter {}",delim));
	}
	name = line.substr(0, pos);
	trim(name);
	value = line.substr(pos + 1, line.length());
	trim(value);

	return std::make_pair(name, value);

}

void trim(std::string& line) {
	line.erase(0, line.find_first_not_of(" \t\n\r\f\v")); //trim leading white spaces

	int lastNoneSpace = 0;

	for (int i = static_cast<int>(line.length() - 1); i >= 0; --i) { //use int because we want to iterate from the back of the line to the front, and terminate loop when cursor reaches front.
		if (!(line[i] == ' ' || line[i] == '\t' || line[i] == '\n' || line[i] == '\r' || line[i] == '\f' || line[i] == '\v')) {
			lastNoneSpace = i + 1;
			break;
		}
	}

	line.erase(line.begin() + lastNoneSpace, line.end());

}