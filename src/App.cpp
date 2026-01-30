
#include "../header/App.h"
#include "../header/WindowManager.h"
#include "../header/Entity.h"
#include "../header/log.h"

// Simple singleton instance
App* App::Instance()
{
    static App instance;
    return &instance;
}

int App::RunApp()
{
	// Initialize WindowManager and create window 800 x 600
    WindowManager windowManager(SCR_WIDTH, SCR_HEIGHT, "Refactor project");
    if (!windowManager.GLFWInitialize()) {
        LOG_ERROR("Failed to initialize WindowManager");
        return -1;
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

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Clear (set color then clear)
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render entity
        entity.Render();

        // Swap and poll
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

	AppShutdown(); // clean up and terminate GLFW
    return 0;
}

void App::AppShutdown()
{
    LOG_INFO("Shutting down application");
    glfwTerminate(); // single place to call glfwTerminate
}

