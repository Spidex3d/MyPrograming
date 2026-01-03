#include "../../header/log.h"
#include "../../header/mylog.h"
// hashinclude is a preprocessor directive that tells the compiler to include the contents of a file or library



int main() { // main is the entry point for every C++ program
	

	int testVar = 42; // local variable
	LOG_WARNING("This is a warning message " << testVar); // __VA_ARGS__ is a special macro that represents all the arguments passed to the macro
	LOG_ERROR("This is an error message");
	LOG_INFO("This is an info message");
	LOG_DEBUG("This is a debug message");

	myLog("Hello, from your Logging project");
	std::string name; // declare a variable to hold the user's name
	
	return 0;
}
