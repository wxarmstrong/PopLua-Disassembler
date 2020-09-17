#include "popLocal.h"

namespace popLua {
	void popLocal::setName(std::string newName) {
		name = newName;
	}
	void popLocal::setScope(uint32_t newBegin, uint32_t newEnd) {
		begin = newBegin;
		end = newEnd;
	}
	std::string popLocal::getName() {
		return name;
	}
	uint32_t popLocal::getBegin() {
		return begin;
	}
	uint32_t popLocal::getEnd() {
		return end;
	}
	bool popLocal::operator == (popLocal other) {
		return (length == other.length && name == other.name && begin == other.begin && end == other.end);
	}
}