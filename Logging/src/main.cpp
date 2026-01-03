#include "../../header/log.h"
#include "../../header/mylog.h"
// hashinclude is a preprocessor directive that tells the compiler to include the contents of a file or library

//struct Player
//{
//	// struct are public by default
//	int xPos, yPos; // x - y position
//	int health;		// health points
//	int score;		// player score
//	int speed;		// movement speed
//};

class Player
{
	// class are private by default
public: // public members can be accessed from outside the class visibly
	int xPos, yPos; // x - y position
	int health;		// health points
	int score;		// player score
	int speed;		// movement speed

};


int main() { // main is the entry point for every C++ program
	// Player is an object type
	Player player1;			// create an instance of the object Player class
	player1.xPos = 10;		// set x position
	player1.yPos = 20;		// set y position
	player1.health = 100;	// set health
	player1.score = 0;		// set score
	player1.speed = 5;		// set speed

	myLog("Player1 position: (" + std::to_string(player1.xPos) + ", " + std::to_string(player1.yPos) + ")");
	myLog("Player1 health: " + std::to_string(player1.health));
	myLog("Player1 score: " + std::to_string(player1.score));
	myLog("Player1 speed: " + std::to_string(player1.speed));

	Player player2;			// create another instance of the object Player class
	player2.xPos = 30;		// set x position and so on


	int testVar = 42; // local variable
	LOG_WARNING("This is a warning message " << testVar); // __VA_ARGS__ is a special macro that represents all the arguments passed to the macro

	myLog("Hello, from your Logging project");
	std::string name; // declare a variable to hold the user's name
	
	return 0;
}
