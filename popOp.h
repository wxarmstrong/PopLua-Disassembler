#ifndef POPOP_H
#define POPOP_H

namespace popLua {

	static enum opType : uint8_t {
		MOVE, LOADK, LOADBOOL, LOADNIL, GETUPVAL, GETGLOBAL, GETTABLE, SETGLOBAL,
		SETUPVAL, SETTABLE, NEWTABLE, SELF, ADD, SUB, MUL, DIV,
		POW, MOD, UNM, NOT, SIZ, CONCAT, JMP, EQ,
		LT, LE, TEST, CALL, TAILCALL, RETURN, FORLOOP, TFORLOOP,
		TFORPREP, SETLIST, SETLISTO, CLOSE, ALTSELF, CONSTGLOBAL, CONSTTABLE, DEFGLOBAL,
		DEFTABLE, SETSELFORGLOBAL, GETSELFORGLOBAL, SELFORGLOBAL, CALLSELFORGLOBAL,
		TAILCALLSELFORGLOBAL, INT, BREAK, CLOSURE
	};

	class popOp {
	public:
		void set(std::istream&infile);
		void setloc(uint32_t linenum);
		uint32_t getloc();
		friend std::ostream& operator<<(std::ostream &out, const popOp &right);
		opType opcode;
		uint16_t operandA;
		uint32_t operandB, operandC;
		uint64_t operandBx;
		int64_t operandsBx;
		uint32_t location;
	private:
		
	};
}

#endif