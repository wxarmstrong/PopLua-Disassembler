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

	void popChunk::process(std::vector<std::string>& stringArray) {
		
		int lastLine = getLastLine();
		lineArray.resize(lastLine);

		for (int i = 0; i < numOps; i++) {
			popOp curOp = opArray[i];
			int loc = curOp.getloc();
			std::vector<popOp>*curLine = &lineArray[loc - 1];
			curLine->push_back(opArray[i]);
		}

		for (int i = 0; i < lineArray.size(); i++) {
			std::cout << std::setw(3) << i+1 << " ";
			std::vector<popOp>*curLine = &lineArray[i];
			for (int j = 0; j < curLine->size(); j++) {
				std::cout << (*curLine)[j] << " ";
			}
			std::cout << std::endl;
		}

		registerArray.resize(numLocals);

		for (int i = 0; i < numOps; i++) {
			for (int j = 0; j < numLocals; j++) {
				popLocal curLocal = localArray[j];
				// Local enters scope
				if (curLocal.getBegin() == i) {
					int firstEmpty = 0;
					while (registerArray[firstEmpty].isFull())
					{
						firstEmpty++;
					}
					registerArray[firstEmpty].add(localArray[j]);
					registerArray[firstEmpty].full = true;
				}
				// Local exits scope
				if (curLocal.getEnd() == i) {
					// Find location of local in registerArray
					int locIndex = -1;
					for (int k = 0; k < numOps; k++) {
						if (registerArray[k].contains(curLocal)) {
							registerArray[k].full = false;
							break;
						}
					}
				}
			}
		}

		
		for (int i = 0; i < lastLine; i++) {
			std::vector<popOp>*curLine = &lineArray[i];
			
			popOp curOp1, curOp2, curOp3, curOp4, curOp5;
			int loc;
			std::string line, opB, opC;

			switch ( curLine->size() ) {
			case 0: break;
			case 1:
				curOp1 = (*curLine)[0];
				if (curOp1.opcode == DEFTABLE)
				{
					opB = constantArray[curOp1.operandB - 250].value;
					opC = constantArray[curOp1.operandC - 250].value;
					opB.erase(0, 1);
					opB.pop_back();
					line = "def " + opB + " = " + opC + ";";
					std::cout << line << std::endl;
					stringArray[i] = line;
				}
				break;
			case 2:
				curOp1 = (*curLine)[0];
				curOp2 = (*curLine)[1];

				if (curOp1.opcode == NEWTABLE && curOp2.opcode == DEFTABLE)
				{
					opB = constantArray[curOp2.operandB - 250].value;
					opB.erase(0, 1);
					opB.pop_back();
					line = "def " + opB + " = {};";
					std::cout << line << std::endl;
					stringArray[i] = line;
				}

				
				else if (curOp1.opcode == NEWTABLE && curOp2.opcode == DEFGLOBAL)
				{
					opB = constantArray[curOp2.operandC].value;
					opB.erase(0, 1);
					opB.pop_back();
					line = "def " + opB + " = {};";
					std::cout << line << std::endl;
					stringArray[i] = line;
				}
				

				break;

			case 3:
				curOp1 = (*curLine)[0];
				curOp2 = (*curLine)[1];
				curOp3 = (*curLine)[2];

				if (curOp1.opcode == GETGLOBAL && curOp2.opcode == GETGLOBAL
					&& curOp3.opcode == CALL)
				{
					line = "";
					opB = constantArray[curOp1.operandC].value;
					opB.erase(0, 1);
					opB.pop_back();
					line += opB;
					line += "(";
					opC = constantArray[curOp2.operandC].value;
					opC.erase(0, 1);
					opC.pop_back();
					line += opC;
					line += ");";
					std::cout << line << std::endl;
					stringArray[i] = line;
				}

				break;

			case 4:
				curOp1 = (*curLine)[0];
				curOp2 = (*curLine)[1];
				curOp3 = (*curLine)[2];
				curOp4 = (*curLine)[3];

				if (curOp1.opcode == GETSELFORGLOBAL && curOp2.opcode == LOADNIL
			     && curOp3.opcode == TFORPREP        && curOp4.opcode == TFORLOOP)
				{
					line = "for (";

					line += getReg(4, curOp4.location);

					line += ",";
					std::cout << line << std::endl;
					stringArray[i] = line;


				}
				break;

			case 5:
				curOp1 = (*curLine)[0];
				curOp2 = (*curLine)[1];
				curOp3 = (*curLine)[2];
				curOp4 = (*curLine)[3];
				curOp5 = (*curLine)[4];
				if (curOp1.opcode == SELFORGLOBAL     && curOp2.opcode == SELFORGLOBAL
				 && curOp3.opcode == MOVE             && curOp4.opcode == CALLSELFORGLOBAL
				 && curOp5.opcode == CALLSELFORGLOBAL)
				{
					opB = constantArray[curOp1.operandC - 250].value;
					opB.erase(0, 1);
					opB.pop_back();
					line = opB
						+ "(";

					opB = constantArray[curOp2.operandC - 250].value;
					opB.erase(0, 1);
					opB.pop_back();
					line += opB
						+ "(";

					//local name
					loc = curOp3.getloc();
					opB = getReg(curOp3.operandC + 1, loc);
					opB.pop_back();
					line += opB;

					line += ")";

					line += ");";

					std::cout << line << std::endl;
					stringArray[i] = line;
				}
				break;
			}
			
		}
		

		for (int i = 0; i < registerArray.size(); i++) {
			std::cout << "Register " << std::setw(2) << std::right << i << ": ";
			popRegister*curReg = &registerArray[i];
			for (int j = 0; j < curReg->getSize(); j++) {
				popLocal curLocal = (*curReg).localArray[j];
				std::cout << curLocal.getName() << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (int i = 0; i < numProto; i++) {
			chunkArray[i].process(stringArray);
		}
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
					name += tempChr;
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
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << chunk.numLocals << " locals: " << std::endl;
			for (int i = 0; i < chunk.depth; i++) {
				std::cout << "   ";
			}
			std::cout << "-----------SCOPE: Begin   End" << std::endl;
			for (int i = 0; i < chunk.numLocals; i++) {
				popLocal*curLocal = &(chunk.localArray[i]);
				for (int j = 0; j < chunk.depth; j++) {
					std::cout << "   ";
				}
				std::cout << std::setw(3) << std::right << i << ": " << std::left << std::setw(8) << curLocal->getName();
				std::cout << "       " << curLocal->getBegin() << "      " << curLocal->getEnd();
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		/*
		else {
			std::cout << "No locals" << std::endl; //Sheerly diagnostic
		}
		*/
		if (chunk.numUpvals > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << chunk.numUpvals << " upvals: " << std::endl;
			for (int i = 0; i < chunk.numUpvals; i++) {
				/*
				FILL IN WITH THE REST
				*/
			}
		}
		/*
		else {
			std::cout << "No upvals" << std::endl; //Sheerly diagnostic
		}
		*/
		if (chunk.numConstants > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << chunk.numConstants << " constants: " << std::endl;
		}
		/*
		else {
			std::cout << "No constants" << std::endl; // Sheerly diagnostic
		}
		*/
		for (int i = 0; i < chunk.numConstants; i++) {
			for (int j = 0; j < chunk.depth; j++) {
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << i << ": " << chunk.constantArray[i] << std::endl;
		}
		std::cout << std::endl;

		//Prototypes
		if (chunk.numProto > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << chunk.numProto << " prototypes: " << std::endl << std::endl;
			for (int i = 0; i < chunk.numProto; i++) {
				for (int j = 0; j < chunk.depth + 1; j++) {
					std::cout << "   ";
				}
				std::cout << "Prototype " << std::setw(3) << std::right << i << ": " << std::endl << std::endl;
				std::cout << chunk.chunkArray[i];
			}
		}

		//Operations

		if (chunk.numOps > 0) {
			for (int i = 0; i < chunk.depth; i++) {
				std::cout << "   ";
			}
			std::cout << std::setw(3) << std::right << chunk.numOps << " instructions: " << std::endl;

			for (int i = 0; i < chunk.numOps; i++) {
				for (int j = 0; j < chunk.depth; j++) {
					std::cout << "   ";
				}
				std::cout << std::setw(3) << std::right << i << ": " << chunk.opArray[i] << std::endl;
			}
			std::cout << std::endl;
		}

		return out;
	}
}