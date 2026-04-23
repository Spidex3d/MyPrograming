#include "../header/WindowManager.h"
#include "../header/log.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"   
#include "stb/stb_image.h"
#include "../header/App.h"


// ################################################################## Helper #####################################
static void DestroyFBO(GLuint& fbo, GLuint& color, GLuint& depth) {
    if (depth) { glDeleteRenderbuffers(1, &depth); depth = 0; }
    if (color) { glDeleteTextures(1, &color); color = 0; }
    if (fbo) { glDeleteFramebuffers(1, &fbo); fbo = 0; }
}


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
   


     //Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // ImGuiConfigFlags_DockingEnable
      
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    

    ImGuiStyle& style = ImGui::GetStyle();

	//style.Colors[ImGuiCol_WindowBg].w = 0.8f; // Semi-transparent window background
   // style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 0.31f, 0.31f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.31f, 0.31f, 1.0f); // Semi-transparent window background

	style.WindowRounding = 5.0f; // Rounded corners for windows

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

void WindowManager::MainDockSpace(bool* p_open)
{
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;// I changed this so my scean shows up on start up

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // you can add a bit of padding  
    ImGui::Begin("DockSpace", p_open, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace to the ini file
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    ImGui::End();
}

void WindowManager::MainWindow(GLFWwindow* window)
{
    ImGui::Begin("Main scene");
    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;
    ImGuiIO& io = ImGui::GetIO();

    // draw your main window here, this is where your OpenGL rendering will show up
    int desired_w = static_cast<int>(window_width * io.DisplayFramebufferScale.x);
    int desired_h = static_cast<int>(window_height * io.DisplayFramebufferScale.y);

    if (desired_w > 0 && desired_h > 0) {
        if (desired_w != m_fbWidth || desired_h != m_fbHeight || m_fbo == 0) {
            Rescale_frambuffer((float)desired_w, (float)desired_h);
        }
    }

    if (m_fbo) {
        // Bind FBO and clear
        Bind_Framebuffer();
        glClearColor(0.12f, 0.15f, 0.18f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Call the registered render callback so Engine renders into the bound FBO.
        // If no callback is set, nothing happens (safe).
        if (m_renderCallback) {
            m_renderCallback();
        }

        // Done rendering to FBO
        Unbinde_Frambuffer();

        ImVec2 imagePos = ImGui::GetCursorScreenPos();

        ImGui::Image((void*)(intptr_t)m_fboViewPortTexture,
            ImVec2(window_width, window_height),
            ImVec2(0, 1), ImVec2(1, 0)); // uv0, uv1 flipped for GL

        // draw overlay here, in the same window, after Image()
        if (m_gridDrawCallback) {
            m_gridDrawCallback(imagePos);
        }

    }
    else {
        // fallback: draw empty box or placeholder text
        ImGui::TextWrapped("Frame buffer not initialized.");
    }



	// ################################### Detect right-click for popup menu (existing UI code) ##########################
    if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup("RightClickMenu");

    }

    if (ImGui::BeginPopup("RightClickMenu"))
    {
        if (ImGui::BeginMenu("Add Grid")) {
            if (ImGui::MenuItem("Grid")) {
                // Request engine to add a Obj via action callback
                if (m_actionCallback) m_actionCallback("AddGrid");
            }
            // other menu items...
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Add a Tiles")) {
            if (ImGui::MenuItem("Tile")) {
                // Request engine to add a Obj via action callback
                if (m_actionCallback) m_actionCallback("AddTile");
            }

            if (ImGui::MenuItem("Water")) {
                // Request engine to add a Gltf via action callback
              //  if (m_actionCallback) m_actionCallback("AddTree");
            }
            // other menu items...
            ImGui::EndMenu();
        }

        ImGui::EndPopup();
    }
	ImGui::End();
}



void WindowManager::MainMenuBar(GLFWwindow* window)
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New scene"))
        {

        }
        if (ImGui::MenuItem("Open scene"))
        {
           

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Save scene"))
        {

        }


        if (ImGui::MenuItem("Save As scene"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Exit"))
        {
            glfwSetWindowShouldClose(window, true);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Cut"))
        {

        }
        if (ImGui::MenuItem("Copy"))
        {

        }
        if (ImGui::MenuItem("Paste"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Wire Frame"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (ImGui::MenuItem("Wire Frame off"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Settings"))
    {
        

        ImGui::EndMenu();
    }

    
    if (ImGui::BeginMenu("Help"))
    {
        if (ImGui::MenuItem("ImGui Help")) {
            {
              

               
            }
            
        }
        if (ImGui::MenuItem("Editor Help"))
        {


        }
        ImGui::Separator();
        if (ImGui::MenuItem("About Help"))
        {

        }

        ImGui::EndMenu();
    }


    ImGui::EndMainMenuBar();

    
}

void WindowManager::Create_FrameBuffer()
{
    int w = GetWidth();
    int h = GetHeight();
    if (w <= 0 || h <= 0) return;
    Rescale_frambuffer((float)w, (float)h);
}

void WindowManager::Bind_Framebuffer()
{
    if (!m_fbo) return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glViewport(0, 0, m_fbWidth, m_fbHeight);
}

void WindowManager::Unbinde_Frambuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    int w = GetWidth();
    int h = GetHeight();
    if (w > 0 && h > 0) {
        glViewport(0, 0, w, h);
    }
}

void WindowManager::Rescale_frambuffer(float width, float height)
{
    int w = static_cast<int>(width);
    int h = static_cast<int>(height);
    if (w <= 0 || h <= 0) return;

    // If same size, nothing to do
    if (m_fbo && m_fbWidth == w && m_fbHeight == h) return;

    // Destroy old attachments (if any)
    DestroyFBO(m_fbo, m_fboViewPortTexture, m_fboDepth);

    // Create new color texture
    glGenTextures(1, &m_fboViewPortTexture);
    glBindTexture(GL_TEXTURE_2D, m_fboViewPortTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Optional: clamp
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create depth+stencil renderbuffer
    glGenRenderbuffers(1, &m_fboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, m_fboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // Create framebuffer and attach
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_fboViewPortTexture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_fboDepth);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOG_WARNING("Failed to create framebuffer: status=0x%x", (unsigned)status);
        DestroyFBO(m_fbo, m_fboViewPortTexture, m_fboDepth);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_fbWidth = m_fbHeight = 0;
        return;
    }

    // success
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_fbWidth = w;
    m_fbHeight = h;
    LOG_INFO("Created FBO %u (color=%u depth=%u) size=%dx%d", (unsigned)m_fbo, (unsigned)m_fboViewPortTexture, (unsigned)m_fboDepth, w, h);
}

int WindowManager::GetWidth() const
{
    if (!window) return 0;
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return w;
}

int WindowManager::GetHeight() const
{
    if (!window) return 0;
    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    return h;
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
