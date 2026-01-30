#pragma once
#include "glad\glad.h"
#include "GLFW\glfw3.h"

class App {
public:
    // Returns a pointer to a single App instance (simple singleton).
    static App* Instance();

    // Run the application (initializes subsystems, enters render loop).
    int RunApp();

    // Shutdown and cleanup (called automatically from RunApp before exit).
    void AppShutdown();

private:
    App() = default;
    ~App() = default;

    // non-copyable
    App(const App&) = delete;
    App& operator=(const App&) = delete;
};

