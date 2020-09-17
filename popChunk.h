#ifndef POPCHUNK_H
#define POPCHUNK_H

#include "popOp.h"
#include "popLocal.h"
#include "popUpval.h"
#include "popConstant.h"
#include "popRegister.h"
#include <vector>

namespace popLua {

	class popChunk {
	public:
		popChunk(uint8_t newDepth = 0);
		~popChunk();
		void process(std::vector<std::string>& stringArray);
		int getLastLine();
		void setDepth(uint8_t newDepth = 0);
		friend std::istream& operator >> (std::istream& in, popChunk& chunk);
		friend std::ostream& operator << (std::ostream& out, popChunk& chunk);
		popConstant* constantArray;
		std::string getReg(uint32_t reg, int loc);
	private:
		const uint8_t static INTRO_SIZE = 0x0C;
		uint8_t depth;
		char* intro;
		uint32_t numOps;
		uint32_t numOps_verify;
		popOp* opArray;
		uint32_t numLocals;
		popLocal* localArray;
		uint32_t numUpvals;
		popUpval* upvalArray;
		uint32_t numConstants;
		
		uint32_t numProto;
		popChunk* chunkArray;
		std::vector<popRegister> registerArray;
		std::vector<std::vector<popOp>> lineArray;
	};
}

#endif