#ifndef POPREGISTER_H
#define POPREGISTER_H

#include <vector>
#include "popLocal.h"

namespace popLua {

	class popRegister{
	public:
		popRegister();
		bool isFull();
		void add(popLocal local);
		bool contains(popLocal local);
		int getSize();
		friend std::ostream& operator << (std::ostream& out, popRegister& reg);
		bool full;
		std::vector<popLocal> localArray;
	};
}

#endif