#include <iostream>
#include <fstream>

#include "popFile.h"

using namespace popLua;

std::vector <std::string> wholeLineArray;

int main(int argc, char* argv[]) {

	if (argc == 1) {
		std::cout << "Requires a PopLua .luc file to be processed.";
	}
	else {
		popFile file;
		std::ifstream in(argv[1], std::ios::binary);
		in >> file;
		file.process();
		std::cout << file;
		in.close();
	}
	std::cout << std::endl;

	system("Pause");
}