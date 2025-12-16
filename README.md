introductory comments
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
 next create a new cpp file inside src called main.cpp by right clicking src and selecting add new item. 
 Change font size to 16 in tools -> options -> environment -> fonts and colors
 If this helped, please subscribe — I will upload slow, gentle lessons as often as I can.

Useful beginners information

// This is comment style number 1 for single line comments.

/*This is comment style number 2
   It can span multiple lines
*/

 ctrl + alt + L Open solution explorer.
 
 ctrl + D Duplicate a line or selection.
 
 ctrl + F Open find dialog.
 
 Hold down Alt and drag to create a rectangular selection.
 
Lesson 1
 ```
 #include <iostream>

 int main() {
  std::cout << "Hello World!" << std::endl;
  return 0;
}
  ```
Lesson 2
```
#include <iostream>
#include <string>


int main() {
	std::string name;
	std::cout << "Hello, from your first project!" << std::endl;
	std::cout << "What's your name" << std::endl;
	std::getline(std::cin, name); // get user input including spaces
	std::cout << "Hello, " << name << " Welcome to C++" << std::endl;
	return 0;
}
```
Lesson 3
```
#include <iostream>
#include <string>

int main() {
	int age = 68; // integer variable
	char key = 'A'; // character variable
	float position = 1.5f; // floating point variable
	bool isRunning = false; // boolean variable

	std::string name;
	std::cout << "Hello, from your first project!" << std::endl;
	std::cout << "What's your name" << std::endl;
	std::getline(std::cin, name); // get user input including spaces
	std::cout << "Hello, " << name << " your age is " << age << std::endl;
	std::cout << "Key is " << key << std::endl;
	std::cout << "Your position is " << position << std::endl; // added line for position
	std::cout << " isRunning " << isRunning << std::endl; // added line for position
	return 0;
}
```
