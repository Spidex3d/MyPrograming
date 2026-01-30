#pragma once
#include "glad/glad.h" // include the glad header file
#include "GLFW/glfw3.h" // include the GLFW header file

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;

class WindowManager
{
public:
	// Constructor and Destructor
    WindowManager(int width = SCR_WIDTH, int height = SCR_HEIGHT, const char* title = "Window");
    ~WindowManager();

    // Initialize GLFW, create window and initialize GLAD
    bool GLFWInitialize();
	// Get the created GLFW window
    GLFWwindow* GetWindow() const;

private:
	GLFWwindow* window = nullptr;	// pointer to GLFW window
	int width{ 0 };                 // window width
	int height{ 0 };                // window height
	const char* title{ nullptr };   // window title
};
