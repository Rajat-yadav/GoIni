// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "goini.h"
#include <iostream>


int main()
{
	GoIni s("test.ini");
	char* sd = s.getCString("test", "poke", "ldkldsdd");
	std::string ds = s.getCString("test", "roki", "dsass");
	double x = s.getDouble("test", "intv", 0.23);
	std::cout << ds << std::endl;
	if (ds == "dsass")
	{
		std::cout << s.getError() << std::endl;
	}
	std::cout << x << std::endl;
	std::cout << sd;
	system("PAUSE");
    return 0;
}

