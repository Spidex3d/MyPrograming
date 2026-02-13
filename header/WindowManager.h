#pragma once
#include "glad/glad.h" // include the glad header file
#include "GLFW/glfw3.h" // include the GLFW header file

constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;
constexpr const char* TITLE = "Add Icon to project"; // new 
constexpr const char* ICON_PATH = "textures/github.jpg"; // new


class WindowManager
{
public:
	// Constructor and Destructor												house keeping functions
    WindowManager(int width = SCR_WIDTH, int height = SCR_HEIGHT, const char* title = TITLE);
    ~WindowManager();

    // Initialize GLFW, create window and initialize GLAD
    bool GLFWInitialize();
	void GLFWIcon();  // new function to set the window icon
	// ImGui setup functins
	void ImGuiInitialize(GLFWwindow* window); // new function to initialize ImGui
	void ImGuiNewFrame(GLFWwindow* window); // new function to start a new ImGui frame
	void ImGuiRender(GLFWwindow* window); // new function to render ImGui
	void ImGuiShutdown(); // new function to clean up ImGui resources

	// Get the created GLFW window
    GLFWwindow* GetWindow() const;

private:
	GLFWwindow* window = nullptr;	// pointer to GLFW window
	int width{ 0 };                 // window width
	int height{ 0 };                // window height
	const char* title{ nullptr };   // window title
};
