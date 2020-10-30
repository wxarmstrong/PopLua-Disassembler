#include <cassert>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include "popChunk.h"

namespace popLua {

	popChunk::popChunk(uint8_t newDepth) {
		depth = newDepth;
	}

	popChunk::~popChunk() {
		delete[] intro;
		delete[] opArray;
		delete[] localArray;
		delete[] upvalArray;
		delete[] constantArray;
		delete[] chunkArray;
	}

	int popChunk::getLastLine() {
		int lastLine = 0;
		
		for (int i = 0; i < numOps; i++) {
			popOp curOp = opArray[i];
			int curLine = curOp.getloc();
			if (curLine > lastLine) {
				lastLine = curLine;
			}
		}
		
		for (int i = 0; i < numProto; i++) {
			popChunk*curProto = &chunkArray[i];
			
			int curLine = curProto->getLastLine();
			if (curLine > lastLine) {
				lastLine = curLine;
			}
			
		}
		
		return lastLine;
	}

	std::string popChunk::getReg(uint32_t reg, int loc) {
		std::string result = "R(";
		result += 48+reg;
		result += ")";
		popRegister curRegister = registerArray[reg];
		std::vector<popLocal> localArray = curRegister.localArray;

		for (int i = 0; i < localArray.size(); i++) {
			popLocal curLocal = localArray[i];
			if (loc >= curLocal.getBegin() && loc <= curLocal.getEnd())
			{
				result = curLocal.getName();
				break;
			}
		}
		
		return result;
	}

	void popChunk::setDepth(uint8_t newDepth) {
		depth = newDepth;
	}

	std::istream& operator >> (std::istream& in, popChunk& chunk) {
		chunk.intro = new char[popChunk::INTRO_SIZE];
		in.read(chunk.intro, popChunk::INTRO_SIZE);

		in.read(reinterpret_cast<char *>(&chunk.numOps), sizeof(chunk.numOps));
		chunk.opArray = new popOp[chunk.numOps];

		for (int i = 0; i < chunk.numOps; i++) {
			uint32_t linenum;
			in.read(reinterpret_cast<char *>(&linenum), sizeof(linenum));
			chunk.opArray[i].setloc(linenum);
		}

		in.read(reinterpret_cast<char *>(&chunk.numLocals), sizeof(chunk.numLocals));
		chunk.localArray = new popLocal[chunk.numLocals];

		if (chunk.numLocals > 0) {
			for (int i = 0; i < chunk.numLocals; i++) {
				popLocal curLocal;
				uint32_t length;
				std::string name = "";
				char tempChr;
				in.read(reinterpret_cast<char *>(&length), sizeof(length));
				for (int j = 0; j < length; j++) {
					in.read(reinterpret_cast<char *>(&tempChr), 1);
					if (j != length - 1)
						name += tempChr; // Don't include null char
				}
				curLocal.setName(name);

				uint32_t begin, end;
				in.read(reinterpret_cast<char *>(&begin), sizeof(begin));
				in.read(reinterpret_cast<char *>(&end), sizeof(end));
				curLocal.setScope(begin, end);

				chunk.localArray[i] = curLocal;
			}
		}

		in.read(reinterpret_cast<char *>(&chunk.numUpvals), sizeof(chunk.numUpvals));
		chunk.upvalArray = new popUpval[chunk.numUpvals];

		if (chunk.numUpvals > 0) {
			;
			for (int i = 0; i < chunk.numUpvals; i++) {
				popUpval curUpval;
				uint32_t length;

				std::string name = "";

				char tempChr;

				in.read(reinterpret_cast<char *>(&length), sizeof(length));

				for (int j = 0; j < length; j++) {
					in.read(reinterpret_cast<char *>(&tempChr), 1);
					name += tempChr;
				}
				curUpval.setName(name);
				chunk.upvalArray[i] = curUpval;
			}
		}

		in.read(reinterpret_cast<char *>(&chunk.numConstants), sizeof(chunk.numConstants));
		chunk.constantArray = new popConstant[chunk.numConstants];
		if (chunk.numConstants > 0) {
			for (int i = 0; i < chunk.numConstants; i++) {
				in >> chunk.constantArray[i];
			}
		}

		//Prototypes
		in.read(reinterpret_cast<char *>(&chunk.numProto), sizeof(chunk.numProto));
		chunk.chunkArray = new popChunk[chunk.numProto];

		if (chunk.numProto > 0) {
			for (int i = 0; i < chunk.numProto; i++) {
				char* buffer = new char[4];
				in.read(buffer, 4);
				delete[] buffer;
				in >> chunk.chunkArray[i];
				chunk.chunkArray[i].setDepth(chunk.depth + 1);
			}
		}

		//Operations
		in.read(reinterpret_cast<char *>(&chunk.numOps_verify), sizeof(chunk.numOps_verify));
		assert(chunk.numOps == chunk.numOps_verify);

		if (chunk.numOps > 0) {
			for (int i = 0; i < chunk.numOps; i++) {
				popOp*curOp = &(chunk.opArray[i]);
				(*curOp).set(in);
			}
		}

		return in;
	}

	std::ostream& operator << (std::ostream& out, popChunk& chunk) {

		if (chunk.numLocals > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << chunk.numLocals << " locals: " << std::endl;
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << "-----------SCOPE: Begin   End" << std::endl;
			for (int i = 0; i < chunk.numLocals; i++) {
				popLocal*curLocal = &(chunk.localArray[i]);
				for (int j = 0; j < chunk.depth; j++) {
					out << "   ";
				}
				out << std::setw(3) << std::right << i << ": " << std::left << std::setw(8) << curLocal->getName();
				out << "       " << curLocal->getBegin() << "      " << curLocal->getEnd();
				out << std::endl;
			}
			out << std::endl;
		}
		/*
		else {
			out << "No locals" << std::endl; //Sheerly diagnostic
		}
		*/
		if (chunk.numUpvals > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << chunk.numUpvals << " upvals: " << std::endl;
			for (int i = 0; i < chunk.numUpvals; i++) {
				/*
				FILL IN WITH THE REST
				*/
			}
		}
		/*
		else {
			out << "No upvals" << std::endl; //Sheerly diagnostic
		}
		*/
		if (chunk.numConstants > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << chunk.numConstants << " constants: " << std::endl;
		}
		/*
		else {
			out << "No constants" << std::endl; // Sheerly diagnostic
		}
		*/
		for (int i = 0; i < chunk.numConstants; i++) {
			for (int j = 0; j < chunk.depth; j++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << i << ": " << chunk.constantArray[i] << std::endl;
		}
		out << std::endl;

		//Prototypes
		if (chunk.numProto > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << chunk.numProto << " prototypes: " << std::endl << std::endl;
			for (int i = 0; i < chunk.numProto; i++) {
				for (int j = 0; j < chunk.depth + 1; j++) {
					out << "   ";
				}
				out << "Prototype " << std::setw(3) << std::right << i << ": " << std::endl << std::endl;
				out << chunk.chunkArray[i];
			}
		}

		//Operations

		if (chunk.numOps > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				out << "   ";
			}
			out << std::setw(3) << std::right << chunk.numOps << " instructions: " << std::endl;

			for (int i = 0; i < chunk.numOps; i++) {
				for (int j = 0; j < chunk.depth; j++) {
					out << "   ";
				}
				out << std::setw(3) << std::right << i << ": " << chunk.opArray[i] << std::endl;
			}
			out << std::endl;
		}

		return out;
	}
}