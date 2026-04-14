#include "../header/App.h"
#include <glad/glad.h>
#include "../header/WindowManager.h"
#include "../header/Entity.h"
#include "../header/log.h"
#include <imgui/imgui.h>



Entity entity;
// Simple singleton instance
App* App::Instance()
{
    static App instance;
    return &instance;
}


void App::SetActionCallback(const std::string& cmd)
{
    if (cmd == "AddTile") {
		// place at center by default later set to a grid square under mouse cursor
        AddPlane(glm::vec3(0.0f, 0.0f, 0.0f));
    }
   
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
   
    windowManager.m_renderCallback = [this, &entity]()
    {
        for (auto& obj : m_entities)
        {
            entity.DrawGameObj(obj.get());
        }
        
    };

    windowManager.m_actionCallback = [this](const std::string& cmd)
    {
        SetActionCallback(cmd);
    };
    // ################################################# New bit ################################

    bool showGUI = true;
	bool show_dockspace = true;
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
        if (showGUI) {
            ImGui::Begin("Scene Collection"); // create a simple ImGui window (optional)


            for (int i = 0; i < (int)m_entities.size(); ++i) {
                GameObj* obj = m_entities[i].get();
                if (!obj) continue;

                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
               // if (m_selectedEntityIndex == i)
                 //   node_flags |= ImGuiTreeNodeFlags_Selected;
                // ICON_FA_TRASH_ALT ICON_FA_PLUS ICON_FA_EDIT


                // Display name + id
                // choose icon based on visibility (requires FA icons loaded)
                //const char* visibilityIcon = obj->isVisible ? ICON_FA_EYE : ICON_FA_EYE_SLASH;

                // display name + id with icon prefix, keep unique ID suffix
                std::string displayName = obj->entName.empty() ? ("Entity " + std::to_string(obj->entId)) : obj->entName;
               std::string label = std::string(displayName + "##" + std::to_string(obj->entId));
                //std::string label = std::string(displayName) + " " + visibilityIcon + "##" + std::to_string(obj->entId);

                // render the tree node (leaf)
                ImGui::TreeNodeEx(label.c_str(), node_flags);


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

void App::AddPlane(const glm::vec3& pos)
{
   /* if (!m_entity) return;
    m_entity->CreatePlane(m_entities, m_currentEntityIndex, m_planeObjIdx, pos);
    m_selectedEntityIndex = static_cast<int>(m_entities.size()) - 1;
    ImGui::SetWindowFocus("Object Inspector");*/
    int newId = static_cast<int>(m_entities.size());
    int planeObjIdx = 0;

    for (const auto& e : m_entities)
    {
        if (e && e->entTypeID == OBJ_PLANE)
            ++planeObjIdx;
    }

    auto plane = std::make_unique<PlaneModel>(newId, "Tile", planeObjIdx);
    plane->position = pos;

    m_entities.push_back(std::move(plane));

    LOG_INFO("Added Plane/Tile. Total entities: " << m_entities.size());


}

void App::AppShutdown()
{
    LOG_INFO("Shutting down application");
    glfwTerminate(); // single place to call glfwTerminate
}

