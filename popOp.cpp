#include <iostream>
#include <iomanip>
#include <fstream>
#include <type_traits>
#include <cassert>
#include <string>
#include "popOp.h"

using namespace std;

namespace popLua {

	//Opcode names retrieved from .exe
	string opName[] = { "MOVE",    "LOADK",          "LOADBOOL",       "LOADNIL","GETUPVAL","GETGLOBAL",  "GETTABLE",  "SETGLOBAL",
						"SETUPVAL","SETTABLE",       "NEWTABLE",       "SELF",   "ADD",     "SUB",        "MUL",       "DIV",
						"POW",     "MOD",            "UNM",            "NOT",    "SIZ",     "CONCAT",     "JMP",       "EQ",
						"LT",      "LE",             "TEST",           "CALL",   "TAILCALL","RETURN",     "FORLOOP",   "TFORLOOP",
						"TFORPREP","SETLIST",        "SETLISTO",       "CLOSE",  "ALTSELF", "CONSTGLOBAL","CONSTTABLE","DEFGLOBAL",
						"DEFTABLE","SETSELFORGLOBAL","GETSELFORGLOBAL","SELFORGLOBAL","CALLSELFORGLOBAL",
						"TAILCALLSELFORGLOBAL", "INT", "BREAK", "CLOSURE" };

	typedef enum addrMode : uint8_t { A, AB, ABx, AsBx, AC, ABC, sBx, B, C, BC };

	//I filled these arguments in based on Lua 5.1 and/or 5.3
	//Opcodes unique to PopLua are marked (worked out manually - could be wrong)

	addrMode opAddr[] = { AB,  ABx, ABC, AB,   AB,  ABx, ABC, ABx,
		//
						  AB,  ABC, ABC, ABC,  ABC, ABC, ABC, ABC,
		//
						  ABC, ABC, AB,  AB,   AB,  ABC, sBx, ABC,
		//		  	                           *
						  ABC, ABC, ABC,ABC,  ABC, AB,   AsBx, ABx,
		//			                ^ AC should work but perhaps they changed it
						  AsBx,   ABC, ABC,   A,    ABC, ABC,   ABC,   ABx,
		//                *         *          *    *    *    *			  
						  ABC,  AC,  AC,  ABC,   ABC, AB,   AB,   A,   ABx };
	//                *    *    *    *     *    *    *    *


	void popOp::set(std::istream&infile) {

		uint32_t opRaw;
		infile.read(reinterpret_cast<char *>(&opRaw), sizeof(opRaw));

		opcode = static_cast<opType>(opRaw & 0x0000003F);
		assert(opcode < 49);
		operandA = (opRaw & 0xFF000000) >> 24;
		operandB = (opRaw & 0x00FF8000) >> 15;
		operandC = (opRaw & 0x00007FC0) >> 6;
		operandBx = (opRaw & 0x00FFFFC0) >> 6;
		operandsBx = (opRaw & 0x00FFFFC0) >> 6;

		if (operandsBx > 131072) {
			operandsBx = operandsBx - 131071;
		}
		else
		{
			operandsBx = 131071 - operandsBx;
			operandsBx *= -1;
		}


	}

	void popOp::setloc(uint32_t linenum) {
		location = linenum;
	}

	uint32_t popOp::getloc() {
		return location;
	}

	std::ostream& operator<<(std::ostream &out, const popOp &right) {
		out << std::setw(3) << std::left << right.location << std::setw(20) << left << opName[right.opcode];
		switch (opAddr[right.opcode]) {

			//Imaginary - Used purely for testing purposes
		case (B):
			assert(right.operandA == 0 && right.operandC == 0);
			break;
		case (C):
			assert(right.operandA == 0 && right.operandB == 0);
			break;
		case (BC):
			assert(right.operandA == 0);
			break;

		case (A):
			assert(right.operandB == 0 && right.operandC == 0);
			out << std::setw(3) << right.operandA << "        ";
			break;
		case (AB):
			assert(right.operandC == 0);
			out << std::setw(3) << right.operandA << " " << std::setw(3) << right.operandB << "    ";
			break;
		case (ABx):
			out << std::setw(3) << right.operandA << " " << std::setw(3) << right.operandBx << "    ";
			break;
		case (AsBx):
			out << std::setw(3) << right.operandA << " " << std::setw(3) << right.operandsBx << "    ";
			break;
		case (AC):
			assert(right.operandB == 0);
			out << std::setw(3) << right.operandA << "     " << std::setw(3) << right.operandC;
			break;
		case (ABC):
			out << std::setw(3) << right.operandA << " " << std::setw(3) << right.operandB << " " << std::setw(3) << right.operandC;
			break;
		case (sBx):
			assert(right.operandA == 0);
			out << "    " << std::setw(3) << right.operandsBx << "        ";
			break;
		default:
			assert(false);
			break;
		}
		out << " ";

		switch (right.opcode) {
		case (MOVE):
			out << "R(" << setw(2) << std::right << right.operandA << ") := R(" << setw(2) << std::right << right.operandB << ")";
			break;
		case (LOADK):
			out << "R(" << setw(2) << std::right << right.operandA << ") := Kst(" << right.operandBx << ")";
			break;
		case (NEWTABLE):
			out << "R(" << setw(2) << std::right << right.operandA << ") := {} (size = " << right.operandB << "," << right.operandC << ")";
			break;
		case (SETTABLE):
			out << "R(" << setw(2) << std::right << right.operandA << ")[" << setw(2) << std::right << right.operandB << "] := R(" << setw(2) << std::right << right.operandC << ")";
			break;
		case (GETTABLE):
			out << "R(" << setw(2) << std::right << right.operandA << ") := " << setw(2) << std::right << right.operandB << "[" << setw(2) << std::right << right.operandC << "]";
			break;
		case (DEFGLOBAL):
			out << "G(" << setw(2) << std::right << right.operandA << ") := R(" << setw(2) << std::right << right.operandC << ")";
			break;
		case (DEFTABLE):
			out << "R(" << setw(2) << std::right << right.operandA << ")[" << right.operandB << "] :=";
			if (right.operandC >= 250) {
				out << "Kst(" << setw(2) << std::right << (right.operandC - 250) << ")";
			}
			else
			{
				out << "R(" << setw(2) << std::right << right.operandC << ")";
			}

			break;
		case (GETGLOBAL):
			out << "R(" << setw(2) << std::right << right.operandA << ") := G(" << setw(2) << std::right << right.operandBx << ")";
			break;
		case (CALL):
			out << "CALL R(" << setw(2) << std::right << right.operandA << ")";
			break;
		}
		return out;
	}
}