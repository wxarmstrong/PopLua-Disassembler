#include <cassert>
#include <string>
#include <iostream>
#include "popConstant.h"

namespace popLua {

	void popConstant::setValue(std::istream& infile) {

		double valFloat;
		int32_t valInt;
		uint32_t strLen;
		std::string temp = "";
		char tempChr;

		switch (type) {
		case nil:
			value = "nil";
			break;
		case numFloat:
			infile.read(reinterpret_cast<char *>(&valFloat), sizeof(valFloat));
			value = std::to_string(valFloat);
			value.erase(value.find_last_not_of('0') + 1, std::string::npos);
			break;
		case numInt:
			infile.read(reinterpret_cast<char *>(&valInt), sizeof(valInt));
			value = std::to_string(valInt);
			break;
		case str:
			infile.read(reinterpret_cast<char *>(&strLen), sizeof(strLen));
			for (int i = 0; i < strLen; i++) {
				infile.read(reinterpret_cast<char *>(&tempChr), 1);
				temp += tempChr;
			}
			value = "\"" + temp.substr(0, strLen - 1) + "\"";
			break;
		default:
			std::cout << std::hex << std::endl;
			assert(false);
		}
	}

	std::string popConstant::getValue() const {
		return value;
	}

	std::istream& operator >> (std::istream& in, popConstant& constant) {
		in.read(reinterpret_cast<char *>(&constant.type), sizeof(constant.type));
		constant.setValue(in);
		return in;
	}

	std::ostream& operator << (std::ostream& out, const popConstant& constant) {
		std::cout << constant.value;
		return out;
	}

}