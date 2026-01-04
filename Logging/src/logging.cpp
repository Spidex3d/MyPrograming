#include "../../header/log.h"
#include "../../header/mylog.h"
// hashinclude is a preprocessor directive that tells the compiler to include the contents of a file or library



int main() { // main is the entry point for every C++ program
	

	int ErrorVar = 42; // local variable so you could add more context to the log message
	LOG_WARNING("This is a warning message " << ErrorVar); // __VA_ARGS__ is a special macro that represents all the arguments passed to the macro
	LOG_ERROR("This is an error message " << "ERROR " << ErrorVar); // concatenate multiple parts into a single log message and add variable context
	LOG_INFO("This is an info message");
	LOG_TRACE("This is a trace message");
	LOG_DEBUG("This is a debug message");


	myLog("Hello, from your Logging project"); // standard output log message no colors or log levels
	
	return 0;
}
