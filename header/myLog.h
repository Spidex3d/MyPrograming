#pragma once // meanes this file will only be included once in a single compilation
#include <iostream>
#include <string>

void myLog(const std::string& message) {
	std::cout << message << std::endl;
}
