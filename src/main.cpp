#include <iostream>
#include <string>
//using namespace std; // use the standard namespace to avoid prefixing std::
// variable declarations
//global variable
short year; // a short integer variable 2 bytes
long population; // a long integer variable 4 bytes
long long distance; // a long long integer variable 8 bytes
double pi; // a double precision floating point variable 8 bytes 3.14159

int multi(int a, int b) { // function to add two integers and return the result
	return a * b;
}
// OR
void multiVoid(int a, int b) { // function to add two integers and return the result via reference
	int result = multi(a, b); // local variable 
	std::cout << "Result is " << result << std::endl;
}

int main() { // main is the entry point for every C++ program
	int result = multi(3, 5); // local variable 
	std::cout << "3 multiplied by 5 is " << result << std::endl;

	int result1 = multi(4, 5); // local variable 
	std::cout << "4 multiplied by 5 is " << result1 << std::endl;

	int result2 = multi(6, 5); // local variable 
	std::cout << "6 multiplied by 5 is " << result2 << std::endl;

	multiVoid(3, 5); // call the void function
	multiVoid(4, 5); // call the void function
	multiVoid(6, 5); // call the void function
	
}

/* introductory comments
* Why am i making this series?  to show how I am teaching myself C++ over the last 18 months.
* C++ is a grate hobby langauge for older learners.
* you can spend as much time as you like on each concept. 1hrs a day or all day if you like.
* I noticed a lack of beginner friendly C++ tutorials aimed at older learners.
* There are plenty of C++ tutorials out there, but most are aimed at younger audiences.
* Plus learning a programming langauge at an older age keeps the mind active and engaged.
* and lastly, learning C++ if you have A pc is all but free.
All tutorials will be aimed at older learners.
So Many tutorials race through concepts, use small text, or skip steps;
A Slow & Clear C++ series aimed at older learners & pensioners (or anyone who prefers a slower pace)

 installing visual studio community edition is the first step
 first thing is to set show all folders in solution explorer and explain about filters vs folders
 Next crate a new folder called src
 next create a new cpp file inside src called main.cpp by right clicking src and selecting add new item
 change font size to 16 in tools -> options -> environment -> fonts and colors
 If this helped, please subscribe — I will upload slow, gentle lessons as often as I can.
*/

// This is comment style number 1 for single line comments
/*This is comment style number 2
   It can span multiple lines
*/

// ctrl + alt + L Open solution explorer
// ctrl + D Duplicate a line or selection
// ctrl + F Open find dialog
// Hold down Alt and drag to create a rectangular selection

// alt to move lines up and down

/* Lesson 3
// 4 primitive variable declarations the most common ones used
	int age = 68; //  - 2b + 2b give or take a bit
	unsigned int score = 5000; // only positive whole numbers 0 to 4b
	char key = '65'; // a single character variable 1 byte you add a char in single quotes like 'A' or number '50'
	float position = 5.5f; // a floating point variable 4 bytes 5.5f no f for double
	bool isRunning = true; // a boolean variable 1 byte true or false 1 or 0 yes or no

	std::string name; // declare a variable to hold the user's name

	std::cout << "Hello, from your first project" << std::endl;
	std::cout << "What's your name" << std::endl;
	std::getline(std::cin, name); // get the user's name from standard input
	std::cout << "Hello " << name << ", your age is " << age << std::endl;
	std::cout << "key was " << key << std::endl;
	std::cout << "position is " << position << std::endl;
	std::cout << "is it running " << isRunning << std::endl;
	return 0;
*/