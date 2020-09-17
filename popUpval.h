#ifndef POPUPVAL_H
#define POPUPVAL_H

#include <string>

namespace popLua {

	class popUpval {
	public:
		void setName(std::string);
		std::string getName();
	private:
		uint32_t length;
		std::string name;
	};
}

#endif