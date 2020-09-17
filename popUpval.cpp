#include "popUpval.h"

namespace popLua {
	void popUpval::setName(std::string newName) {
		name = newName;
	}
	std::string popUpval::getName() {
		return name;
	}
}