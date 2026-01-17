#include "../header/log.h"
#include "../header/mylog.h"
#include "glad\glad.h" // include the glad header file
#include <GLFW/glfw3.h> // include the GLFW header file
#include "../header/shader.h" // include our shader class
// Ok so this one will be a bit longer because
// we are make our first C++ program with an external library GLFW to create a window
// what is glfw - it's a graphics library framework for creating windows
// with OpenGL contexts and managing input and events
// very useful and easy to use

// go to properties of the project and set the include directory to point to the glfw include folder
// go to properties of the project and set the linker input additional dependencies to include glfw3.lib and opengl32.lib
// also set the library directory to point to the glfw lib folder
// set the latest c++ standard in project properties to c++20

// GLFW https://www.glfw.org/
// glad https://glad.dav1d.de/


int main() { // main is the entry point for every C++ program
	LOG_INFO("Hello, Let's open a window with GLFW");





    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return false;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "My first window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return false;
    }
    else {
		LOG_INFO("GLFW window created successfully");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	// Initialize GLAD before calling any OpenGL function
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD");
		return -1;
	}
	else {
		LOG_INFO("GLAD initialized successfully");
	}

	LOG_TRACE("OpenGL version: " << std::string((const char*)glGetString(GL_VERSION)));

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

	// ############################################# Draw a triangle setup
	
	GLuint VAO, VBO; // declare vertex array object and vertex buffer object

	float vertices[] = { // This is an array of 3 2D vertices (x, y) for a triangle
            /*-1.0f, -1.0f, 
             1.0f, -1.0f, 
             0.0f,  1.0f*/

             - 0.5f, -0.5f,
             0.5f, -0.5f,
             0.0f,  0.5f
    };

	glGenVertexArrays(1, &VAO); // generate a vertex array object
	glBindVertexArray(VAO); // bind the vertex array object

	glGenBuffers(1, &VBO); // generate a vertex buffer object
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the vertex buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // copy the vertex data to the buffer

	//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0); // set the vertex attribute pointer
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


	// #############################################
	// ######################## Arrays setup bit more info ######
			//                     0,    1,       2
	std::string TestArray[] = { "Red", "Green", "Blue" }; // an array of strings
	int intArray[]{ 10, 12, 19, 22 }; //an array of integers
	
	LOG_INFO("Array at index 0 " << TestArray[0]); // access the second element of the array (index 1)
	LOG_INFO("Array at index 1 " << TestArray[1]); // access the second element of the array (index 1)
	LOG_INFO("Array at index 2 " << TestArray[2]); // access the second element of the array (index 1)

	LOG_INFO("Array at index 0 " << intArray[0]); // access the second element of the array (index 1)
	LOG_INFO("Array at index 1 " << intArray[1]); // access the second element of the array (index 1)
	LOG_INFO("Array at index 3 " << intArray[3]); // access the second element of the array (index 1)
	//print out array size
	LOG_INFO("Size of intArray is " << sizeof(intArray) / sizeof(intArray[0])); // calculate the size of the array
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Render here 
        glClear(GL_COLOR_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // set clear color to a dark teal color

		shader.Use();
        shader.SetUniformVec3("uColor", 0.1f, 0.9f, 0.4f); // green-ish
        // ##########
        glBindVertexArray(VAO);
		/*
        The glDrawArrays function takes as its first argument the OpenGL primitive type we would like to draw.
        we pass in GL_TRIANGLES.
        The second argument specifies the starting index of the vertex array we'd like to draw; we just leave this at 0.
        The last argument specifies how many vertices we want to draw,
        which is 3 
        */
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
		// ##########
        
        // Swap front and back buffers 
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    LOG_INFO("Closing our GLFW window");
    glfwTerminate();
	
	
	return 0;
}






//struct Player
//{
//	// struct are public by default
//	int xPos, yPos; // x - y position
//	int health;		// health points
//	int score;		// player score
//	int speed;		// movement speed
// int level;		// player level
// int countdown;
// 
//};

//class Player
//{
//	// class are private by default
//public: // public members can be accessed from outside the class visibly
//	int xPos, yPos; // x - y position
//	int health;		// health points
//	int score;		// player score
//	int speed;		// movement speed
//	int level;		// player level
//	int countdown;
//
//
//};


//Player player1;			// create an instance of the object Player class
//player1.xPos = 10;		// set x position
//player1.yPos = 20;		// set y position
//player1.health = 100;	// set health
//player1.score = 0;		// set score
//player1.speed = 5;		// set speed
//player1.level = 1;      // set level
//player1.countdown = 5;  // set countdown

//myLog("Player1 position: (" + std::to_string(player1.xPos) + ", " + std::to_string(player1.yPos) + ")");
//myLog("Player1 health: " + std::to_string(player1.health));
//myLog("Player1 score: " + std::to_string(player1.score));
//myLog("Player1 speed: " + std::to_string(player1.speed));

//// A conditional statement to check if player1 is alive or dead, > more than 0 is alive
//// Add a brake point on the next line to pause execution and inspect variables
//if (player1.health > 0) {
//	LOG_INFO("Player1 is alive");
//}
//else {
//	LOG_WARNING("Player1 is dead");
//}
//bool colourGreen = true; // boolean variable to indicate if the color is green
//// add a ! to the front of colourGreen to test the else condition
//if (colourGreen) {
//	LOG_INFO("The color is Red");
//}
//else {
//	LOG_WARNING("The color is not Red");
//}
//// switch statement example to check player level
//switch (player1.level) {
//case 0:
//	LOG_INFO("Level is zero");
//	break;
//case 1:
//	LOG_INFO("Level is one");
//	break;
//case 2:
//	LOG_INFO("Level is two");
//	break;
//}

//
//// while loop example to count down from 5
//while (player1.countdown > 0) {
//	LOG_INFO("Countdown: " << player1.countdown);
//	player1.countdown--;
//}
//// when countdown reaches 0 move to next line of code
//LOG_INFO("Start game at level: " << player1.level);

//int i = 0; // local variable for do while loop set to 0
//// do while loop example to count up to 5
//do {
//	LOG_INFO("Game will Start in: " << i);
//	i++; // increment i by 1 each loop iteration
//}
//// condition is checked after the loop body is executed
//while (i < 5);

//// for loop example to count down from 5
//for (int i = 5; i > 0; i--) {
//	LOG_INFO("count down from 5: " << i);
//}
//

//

//int testVar = 42; // local variable
//LOG_INFO("This is the players Speed " << std::to_string(player1.speed)); // __VA_ARGS__ is a special macro that represents all the arguments passed to the macro
//LOG_WARNING("This is a warning message " << testVar); // __VA_ARGS__ is a special macro that represents all the arguments passed to the macro

//myLog("Hello, from your Class and Struct project");


//std::string name; // declare a variable to hold the user's name
//myLog("What's your name");
//std::getline(std::cin, name); // get the user's name from standard input
//myLog("Hello " + name + ", welcome to C++ programming!");


//using namespace std; // use the standard namespace to avoid prefixing std::
// variable declarations
//global variable
//short year; // a short integer variable 2 bytes
//long population; // a long integer variable 4 bytes
//long long distance; // a long long integer variable 8 bytes
//double pi; // a double precision floating point variable 8 bytes 3.14159
//
//int multi(int a, int b) { // function to add two integers and return the result
//	return a * b;
//}
//// OR
//void multiVoid(int a, int b) { // function to add two integers and return the result via reference
//	int result = multi(a, b); // local variable 
//	std::cout << "Result is " << result << std::endl;
//}
//
//int main() { // main is the entry point for every C++ program
//	int result = multi(3, 5); // local variable 
//	std::cout << "3 multiplied by 5 is " << result << std::endl;
//
//	int result1 = multi(4, 5); // local variable 
//	std::cout << "4 multiplied by 5 is " << result1 << std::endl;
//
//	int result2 = multi(6, 5); // local variable 
//	std::cout << "6 multiplied by 5 is " << result2 << std::endl;
//
//	multiVoid(3, 5); // call the void function
//	multiVoid(4, 5); // call the void function
//	multiVoid(6, 5); // call the void function
//	
//}

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