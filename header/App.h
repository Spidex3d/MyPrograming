#pragma once
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include <vector>
#include <memory>
#include "Entity.h"

class App {
public:
    // Returns a pointer to a single App instance (simple singleton).
    static App* Instance();
    void SetActionCallback(const std::string& cmd);
    // Run the application (initializes subsystems, enters render loop).
    int RunApp();

    void AddPlane(const glm::vec3& pos = glm::vec3(0.0f));

    // Shutdown and cleanup (called automatically from RunApp before exit).
    void AppShutdown();

  

private:
    App() = default;
    ~App() = default;

    // non-copyable
    App(const App&) = delete;
    App& operator=(const App&) = delete;


    //std::unique_ptr<App> m_entity;
    std::vector<std::unique_ptr<GameObj>> m_entities;

};

