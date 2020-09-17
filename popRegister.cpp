#include "popRegister.h"

#include <vector>
#include <iostream>
#include "popLocal.h"

namespace popLua {

	popRegister::popRegister() {
		full = false;
	}

	bool popRegister::isFull() {
		return full;
	}

	void popRegister::add( popLocal local ) {
		localArray.push_back(local);
	}

	bool popRegister::contains(popLocal local) {
		for (int i = 0; i < localArray.size(); i++) {
			if (local == localArray[i]) {
				return true;
			}
		}
		return false;
	}

	int popRegister::getSize() {
		return localArray.size();
	}

	std::ostream& operator << (std::ostream& out, const popRegister& reg) {

		return out;
	}
}