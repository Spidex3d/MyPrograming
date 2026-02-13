#include "../header/WindowManager.h"
#include "../header/log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"   
#include "stb/stb_image.h"

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

    //float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor()); // Valid on GLFW 3.3+ only


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

void WindowManager::GLFWIcon()
{
	// Load icon image using stb_image and set it as the GLFW window icon Add stbi_image.h to the project and include it here
	GLFWimage icon[1];
	//icon[0].pixels = stbi_load("textures/icon.png", &icon[0].width, &icon[0].height, 0, 4);
	icon[0].pixels = stbi_load(ICON_PATH, &icon[0].width, &icon[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, icon);
	stbi_image_free(icon[0].pixels);

}

void WindowManager::ImGuiInitialize(GLFWwindow* window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    ImGuiStyle& style = ImGui::GetStyle();

    // Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true); // GLFW & OpenGL3 backend initialization
	const char* glsl_version = "#version 460 core";
    ImGui_ImplOpenGL3_Init(glsl_version);

    
}

void WindowManager::ImGuiNewFrame(GLFWwindow* window)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void WindowManager::ImGuiRender(GLFWwindow* window)
{
	ImGui::Render();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void WindowManager::ImGuiShutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

GLFWwindow* WindowManager::GetWindow() const
{
    return window;
}
