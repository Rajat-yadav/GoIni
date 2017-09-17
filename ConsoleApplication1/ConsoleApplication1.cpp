// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "goini.h"
#include <iostream>


int main()
{
	GoIni s;
	s = "test.ini";
	std::string ds = s.getString("test", "rokii", "dsass");
	std::cout << ds;
	const char* sd = s.getCString("test", "pok", "ldkldsdd");
	std::cout << sd;
	system("PAUSE");
    return 0;
}

