#ifndef POPCONSTANT_H
#define POPCONSTANT_H

#include <string>

namespace popLua {

	typedef enum cType : uint8_t { nil, numFloat = 3, numInt, str };

	class popConstant {
	public:
		void setValue(std::istream& infile);
		std::string getValue() const;
		friend std::istream& operator >> (std::istream& in, popConstant& constant);
		friend std::ostream& operator << (std::ostream& out, const popConstant& constant);
		std::string value;
	private:
		cType type;
		uint32_t length;
	};
}

#endif