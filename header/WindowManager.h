#pragma once
#include "glad/glad.h" // include the glad header file
#include "GLFW/glfw3.h" // include the GLFW header file
#include "imgui/imgui.h" // include the ImGui header file
#include <functional> // for std::function
#include <string>



constexpr int SCR_WIDTH = 800;
constexpr int SCR_HEIGHT = 600;
constexpr const char* TITLE = "Add Viewport Grid"; // Change the project to a 2d Platform Editor 
constexpr const char* ICON_PATH = "textures/github.jpg"; // new



class WindowManager
{
public:
	using RenderCallback = std::function<void()>; // called while FBO is bound so we can render into it
	using ActionCallback = std::function<void(const std::string&)>; // called when UI requests an action
	using GridDrawCallback = std::function<void(ImVec2)>;
	
	// Constructor and Destructor												house keeping functions
    WindowManager(int width = SCR_WIDTH, int height = SCR_HEIGHT, const char* title = TITLE);
    ~WindowManager();

    // Initialize GLFW, create window and initialize GLAD
    bool GLFWInitialize();
	void GLFWIcon();  // new function to set the window icon
	// ImGui setup functins
	void ImGuiInitialize(GLFWwindow* window); // new function to initialize ImGui
	void ImGuiNewFrame(GLFWwindow* window); // new function to start a new ImGui frame
	void MainDockSpace(bool* p_open); // docking space

	// ################################################### New functions for framebuffer management ######################
	void MainWindow(GLFWwindow* window); // main window for drawingthe opengl objects
	void MainMenuBar(GLFWwindow* window); // main menu bar (optional)

	void Create_FrameBuffer();                 // create or recreate the framebuffer using current size
	void Bind_Framebuffer();                  // bind the offscreen FBO for rendering
	void Unbinde_Frambuffer();                // unbind (return to default framebuffer)
	void Rescale_frambuffer(float width, float height); // recreate at given pixel size

	int GetWidth() const;
	int GetHeight() const;

	

	RenderCallback m_renderCallback = nullptr;
	ActionCallback m_actionCallback = nullptr;
	GridDrawCallback m_gridDrawCallback = nullptr;

	// ################################################### End New functions for framebuffer management ######################

	void ImGuiRender(GLFWwindow* window); // new function to render ImGui

	

	void ImGuiShutdown(); // new function to clean up ImGui resources

	// Get the created GLFW window
    GLFWwindow* GetWindow() const;

private:
	GLFWwindow* window = nullptr;	// pointer to GLFW window
	int width{ 0 };                 // window width
	int height{ 0 };                // window height
	const char* title{ nullptr };   // window title

	

	// New members for framebuffer management
	GLuint m_fbo = 0;
	GLuint m_fboViewPortTexture = 0; // color texture
	GLuint m_fboDepth = 0;
	int m_fbWidth = 0;
	int m_fbHeight = 0;

};
