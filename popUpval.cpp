#include "popUpval.h"

namespace popLua {
	void popUpval::setName(std::string newName) {
		name = newName;
	}
	std::string popUpval::getName() {
		return name;
	}
	std::ostream& operator << (std::ostream& out, const popUpval& upval) {
		out << upval.name;
		return out;
	}
}