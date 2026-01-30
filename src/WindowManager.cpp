#include "../header/WindowManager.h"
#include "../header/log.h"

// Constructor initializes members
WindowManager::WindowManager(int width_, int height_, const char* title_)
    : width(width_), height(height_), title(title_), window(nullptr)
{
}
// Destructor cleans up GLFW window
WindowManager::~WindowManager()
{
    if (window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    // Do NOT call glfwTerminate() here — App will call it once on shutdown.
}
// WindowManager to initialize GLFW, create window, and initialize GLAD
bool WindowManager::GLFWInitialize()
{
    if (!glfwInit()) {
        LOG_ERROR("glfwInit failed");
        return false;
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        LOG_ERROR("glfwCreateWindow failed");
        glfwTerminate();
        return false;
    }
    LOG_INFO("GLFW window created successfully");

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // enable VSync (optional)

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        LOG_ERROR("Failed to initialize GLAD");
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        return false;
    }
	// If we reach here, GLAD initialized successfully
    LOG_INFO("GLAD initialized successfully");

    LOG_TRACE("OpenGL version: " << reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    return true;
}

GLFWwindow* WindowManager::GetWindow() const
{
    return window;
}
