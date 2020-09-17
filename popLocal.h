#ifndef POPLOCAL_H
#define POPLOCAL_H

#include <string>

namespace popLua {

	class popLocal {
	public:
		void setName(std::string);
		void setScope(uint32_t newBegin, uint32_t newEnd);
		uint32_t getBegin();
		uint32_t getEnd();
		bool operator == (popLocal other);
		std::string getName();
	private:
		uint32_t length;
		std::string name;
		uint32_t begin;
		uint32_t end;
	};
}

#endif