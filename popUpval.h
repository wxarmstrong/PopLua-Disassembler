#ifndef POPUPVAL_H
#define POPUPVAL_H

#include <iostream>
#include <string>

namespace popLua {

	class popUpval {
	public:
		void setName(std::string);
		std::string getName();
		friend std::ostream& operator << (std::ostream& out, const popUpval& upval);
	private:
		uint32_t length;
		std::string name;
	};
}

#endif