#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "popFile.h"
#include "popChunk.h"

namespace popLua {

	popFile::~popFile() {
		delete[] header;
		delete[] name;
		delete[] chunkArray;
	}

	int popFile::getFileSize() {
		return chunkArray[0].getLastLine();
	}

	std::istream& operator >> (std::istream& in, popFile& file) {
		file.header = new char[popFile::HEADER_SIZE];
		in.read(file.header, popFile::HEADER_SIZE);

		in.read(reinterpret_cast<char *>(&file.nameSize), sizeof(file.nameSize));
		file.name = new char[file.nameSize];
		in.read(file.name, file.nameSize);

		file.chunkArray = new popChunk[1];
		in >> file.chunkArray[0];

		return in;
	}

	std::ostream& operator << (std::ostream& out, popFile& file) {
		out << file.name << std::endl << std::endl;
		out << "Main block" << std::endl << std::endl;
		out << file.chunkArray[0];
		return out;
	}
}