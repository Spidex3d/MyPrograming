#include "../header/App.h"
#include <glad/glad.h>
#include "../header/WindowManager.h"
#include "../header/Entity.h"
#include "../header/log.h"
#include <imgui/imgui.h>

// Simple singleton instance
App* App::Instance()
{
    static App instance;
    return &instance;
}

int App::RunApp()
{
	// Initialize WindowManager and create window 800 x 600
    WindowManager windowManager(SCR_WIDTH, SCR_HEIGHT, TITLE);
    if (!windowManager.GLFWInitialize()) {
        LOG_ERROR("Failed to initialize WindowManager");
        return -1;
    }
    else {
		LOG_INFO("WindowManager initialized successfully");
	    windowManager.GLFWIcon(); // set the window icon (optional)

		// ImGui initialization (optional)
		windowManager.ImGuiInitialize(windowManager.GetWindow());
    }


	// Get the created window
    GLFWwindow* window = windowManager.GetWindow();
    if (!window) {
        LOG_ERROR("WindowManager returned null window");
        return -1;
    }

    // Create and initialize an entity (creates shader, mesh & texture just once)
    Entity entity;
    if (!entity.Initialize(window)) {
        LOG_ERROR("Entity failed to initialize");
        // still run shutdown so glfwTerminate is called
        AppShutdown();
        return -1;
    }

    // ################################################# New bit ################################
    windowManager.m_renderCallback = [&entity]()
    {
        entity.Render();
    };
    // ################################################# New bit ################################

    bool show_hello_window = true;
	bool show_dockspace = true;
	bool show_demo_window = false; // flag to control ImGui demo window (optional)
    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Clear (set color then clear)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        windowManager.ImGuiNewFrame(window); // start new ImGui frame (optional)

       
        windowManager.MainDockSpace(&show_dockspace);
        

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse; // Set ImGui window flags (optional) 
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_None; // Set ImGui window flags (optional) 
        if (show_hello_window) {
            ImGui::Begin("Hello, ImGui!", &show_demo_window, flags); // create a simple ImGui window (optional)

            ImGui::Text("This is a simple ImGui window."); // add some text to the ImGui window (optional)


            if (ImGui::Button("Demo Window")) {
                show_demo_window = !show_demo_window; // Toggle the demo window visibility
            }
            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window); // Show the ImGui demo window
            }

            ImGui::End(); // end the ImGui window (optional)
        }
        // ################################################## Dont forget to call this #################################
		windowManager.MainWindow(window); // set up main ImGui window for OpenGL rendering (optional)
		windowManager.MainMenuBar(window); // set up main menu bar (optional)

        // Render entity
        //entity.Render();

		windowManager.ImGuiRender(window); // render ImGui (optional)

        // Swap and poll
        glfwSwapBuffers(window);
       
    }
	windowManager.ImGuiShutdown(); // clean up ImGui resources (optional)
	AppShutdown(); // clean up and terminate GLFW
    return 0;
}

void App::AppShutdown()
{
    LOG_INFO("Shutting down application");
    glfwTerminate(); // single place to call glfwTerminate
}

