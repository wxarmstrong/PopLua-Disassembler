#ifndef POPFILE_H
#define POPFILE_H

#include "popChunk.h"
#include <iostream>
#include <vector>

namespace popLua {

	class popFile {
	public:
		~popFile();
		void process();
		int getFileSize();
		friend std::istream& operator >> (std::istream& in, popFile &file);
		friend std::ostream& operator << (std::ostream& out, popFile &file);
	private:
		const uint8_t static HEADER_SIZE = 0x17;
		char* header;
		uint32_t nameSize;
		char* name;
		popChunk* chunkArray;
		std::vector<std::string> stringArray;
	};
}

#endif