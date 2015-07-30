#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>

#include "Parser.h"

int main(int argc, char *argv[])
{
//	std::cout << "Please input the file name:" << std::endl;
	if (argc != 2)
	{
		std::cerr << "Usage: " << std::endl;
		return -1;
	}

	Parser ps;
	ps.readFromFile(argv[1]);
	ps.split();
	ps.findCutpoints();
	ps.createNewDataset();
	ps.writeIntoFile();
}