#include "../header/App.h"
#include <glad/glad.h>
#include "../header/WindowManager.h"
#include "../header/Entity.h"
#include "../header/log.h"
//#include <imgui/imgui.h>




Entity entity;
// Simple singleton instance
App* App::Instance()
{
    static App instance;
    return &instance;
}


void App::SetActionCallback(const std::string& cmd)
{
    if (cmd == "AddGrid") {
        // place at center by default later set to a grid square under mouse cursor
        AddGrid(m_rows, m_cols, m_cellSize);
    }

    if (cmd == "AddTile") {
		// place at center by default later set to a grid square under mouse cursor
       // AddPlane(glm::vec2(0.0f, 0.0f));
        if (m_hoveredIndex >= 0 && m_hoveredIndex < m_grid.size())
        {
            m_grid[m_hoveredIndex].tileID = 1;
        }
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

	// ######################################## Add Tile Texture ########################################

    m_tileTexture = std::make_unique<Texture>(TILE_PATH);

    if (!m_tileTexture || !m_tileTexture->IsLoaded())
    {
        LOG_WARNING("Failed to load editor tile texture");
        m_tileTextureID = 0;
    }
    else
    {
        m_tileTextureID = m_tileTexture->ID();
        LOG_INFO("Editor tile texture loaded with ID: " << m_tileTextureID);
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

    windowManager.m_gridDrawCallback = [this](ImVec2 m_imagePos)
    {
        DrawGrid(m_imagePos);
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
        
        // ############################## object editor ###############################
        if (m_selectedEntityIndex >= 0 && m_selectedEntityIndex < (int)m_entities.size()) {
            ImGui::Begin("Object Inspector");

            if (m_selectedEntityIndex >= 0 && m_selectedEntityIndex < (int)m_entities.size()) {
                GameObj* selected = m_entities[m_selectedEntityIndex].get();
                if (selected) {
                    // Name / rename
                    char nameBuf[128];
                    //strncpy(nameBuf, selected->entName.c_str(), sizeof(nameBuf));
                    strncpy_s(nameBuf, selected->entName.c_str(), sizeof(nameBuf));
                    nameBuf[sizeof(nameBuf) - 1] = '\0';
                    if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
                        selected->entName = std::string(nameBuf);
                    }
                  //  ImGui::TextColored(COLOR_LIGHTBLUE, ICON_FA_EDIT "  Editor");
                    // Position
                    float pos[3] = { selected->position.x, selected->position.y};
                    if (ImGui::InputFloat2("Position", pos)) {
                        selected->position = glm::vec2(pos[0], pos[1]);
                    }

                    // Rotation (Euler degrees for editing)
                    // store rotation in radians or degrees depending on your representation, this example uses degrees
                    /*float rotDeg[3] = {
                        glm::degrees(selected->rotation.x),
                        glm::degrees(selected->rotation.y),
                        glm::degrees(selected->rotation.z)
                    };*/
                    /*if (ImGui::InputFloat3("Rotation (deg)", rotDeg)) {
                        selected->rotation = glm::vec3(glm::radians(rotDeg[0]), glm::radians(rotDeg[1]), glm::radians(rotDeg[2]));
                    }*/

                    // Scale
                    float sc[2] = { selected->scale.x, selected->scale.y };
                    if (ImGui::InputFloat2("Scale", sc)) {
                        selected->scale = glm::vec2(sc[0], sc[1]);
                    }

                    // Update modelMatrix using TRS (make sure order is correct for your math)
                   // selected->modelMatrix = glm::translate(glm::mat4(1.0f), selected->position);
                    // apply rotation (if you use Euler -> convert to quat / rotate)
                   // selected->modelMatrix = glm::rotate(selected->modelMatrix, selected->rotation.x, glm::vec3(1, 0, 0));
                   /// selected->modelMatrix = glm::rotate(selected->modelMatrix, selected->rotation.y, glm::vec3(0, 1, 0));
                    //selected->modelMatrix = glm::rotate(selected->modelMatrix, selected->rotation.z, glm::vec3(0, 0, 1));
                   //selected->modelMatrix = glm::scale(selected->modelMatrix, selected->scale);

                    ImGui::SeparatorText("Scene Properties");
                    ImGui::Text("Gameplay Properties");
                    //ImGui::InputInt("Points", &selected->entPoints);
                    //if (ImGui::Checkbox("Active", &selected->isActive)) { /* optionally handle enable/disable */ }
                    //if (ImGui::Checkbox("Rotate Y", &selected->isRotateY)) {
                        // toggling rotateY will cause the object to start/stop rotating in the render loop
                      //  selected->isRotateY = selected->isRotateY; // just to emphasize the change happens immediately
                   // }
                    //if (ImGui::Checkbox("Health Pack", &selected->isHealthPack)) {
                        // show health points input only if flagged
                    //}
                    //if (selected->isHealthPack) {
                     //   ImGui::InputInt("Health Pack Points", &selected->HealthPackPoints);
                    //}
                    //ImGui::Checkbox("Dangerous", &selected->isDangerous);
                    //ImGui::Checkbox("Collidable", &selected->isCollidable);
                    if (ImGui::Checkbox("Visible", &selected->isVisible)) {
                        // toggling visible will affect rendering next frame
                    }

                    ImGui::SeparatorText("Texture on selected object");

                    // show path or "None"
                    if (!selected->texPath.empty()) {
                        ImGui::TextWrapped("Path: %s", selected->texPath.c_str());
                    }
                    else {
                        ImGui::Text("Texture: None");
                    }

                    // Preview (if texture present)
                    if (selected->tex_ID != 0) {
                        ImGui::Text("Preview:");
                        ImGui::Image((void*)(intptr_t)selected->tex_ID, ImVec2(128, 128));

                    }

                    // Change texture button
                    if (ImGui::Button("Change Texture")) {
                        // Blocking Win32 dialog - returns UTF-8 path (your openFileDialog returns std::string)
                        std::string path;
                        if (window) {
                           // path = window->openFileDialog();
                        }

                        if (!path.empty()) {
                           /* if (!m_entity->SetTextureForGameObj(selected, path)) {
                                LOG_WARNING("Failed to set texture for entity " << selected->entId);
                            }*/
                        }
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Clear Texture")) {
                        // Clear/unload texture
                       // m_entity->SetTextureForGameObj(selected, "");
                    }

                    // Buttons for convenience
                    if (ImGui::Button("Focus")) {
                        // implement camera focus in future: center camera on selected->position
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Delete")) {
                        m_entities.erase(m_entities.begin() + m_selectedEntityIndex);
                        m_selectedEntityIndex = -1;
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Exit")) {
                        // close object inspector - editor
                        m_selectedEntityIndex = -1;
                    }
                }
            }
            else {
                ImGui::Text("No object selected");
            }

            

            ImGui::End();
        }
        // ################################################ End object editor ###############################


        

        // ################################################ Scene Collection ###############################

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse; // Set ImGui window flags (optional) 
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_None; // Set ImGui window flags (optional) 
        if (showGUI) {
            ImGui::Begin("Scene Collection"); // create a simple ImGui window (optional)


            for (int i = 0; i < (int)m_entities.size(); ++i) {
                GameObj* obj = m_entities[i].get();
                if (!obj) continue;

                ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
                if (m_selectedEntityIndex == i) 
                  node_flags |= ImGuiTreeNodeFlags_Selected;
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

                

                if (ImGui::IsItemClicked()) {
                     m_selectedEntityIndex = i;
                }

            }    
            ImGui::End(); // end the ImGui window (optional)
        }
        // ################################################ End Scene Collection ###############################


        // ################################################## Dont forget to call this #################################
		windowManager.MainWindow(window); // set up main ImGui window for OpenGL rendering (optional)
		windowManager.MainMenuBar(window); // set up main menu bar (optional)

        
		windowManager.ImGuiRender(window); // render ImGui (optional)

        // Swap and poll
        glfwSwapBuffers(window);
       
    }
	windowManager.ImGuiShutdown(); // clean up ImGui resources (optional)
	AppShutdown(); // clean up and terminate GLFW
    return 0;
}

void App::AddGrid(int rows, int cols, float cellSize)
{
    m_rows = rows;
    m_cols = cols;
    m_cellSize = cellSize;

    m_grid.clear();

    for (int row = 0; row < m_rows; ++row)
    {
        for (int col = 0; col < m_cols; ++col)
        {
            GridCell cell;
            cell.row = row;
            cell.col = col;
            cell.gridIndex = row * m_cols + col;
            cell.worldPos = glm::vec2(col * m_cellSize, row * m_cellSize);
            cell.tileID = -1;

            m_grid.push_back(cell);

           
        }
    }
}


void App::DrawGrid(ImVec2 m_imagePos)
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (!draw_list) return;

    ImVec2 mousePos = ImGui::GetMousePos();

    m_hoveredRow = -1;
    m_hoveredCol = -1;
    m_hoveredIndex = -1;

    // 1. Draw placed tile images first
    if (m_tileTextureID != 0)
    {
        for (auto& cell : m_grid)
        {
            if (cell.tileID == -1)
                continue;

            float x = m_imagePos.x + cell.col * m_cellSize;
            float y = m_imagePos.y + cell.row * m_cellSize;

            draw_list->AddImage(
                (ImTextureID)(intptr_t)m_tileTextureID,
                ImVec2(x, y),
                ImVec2(x + m_cellSize, y + m_cellSize),
                ImVec2(0, 1),
                ImVec2(1, 0)
            );
        }
    }

    // 2. Draw grid lines and hover highlight
    for (auto& cell : m_grid)
    {
        float x = m_imagePos.x + cell.col * m_cellSize;
        float y = m_imagePos.y + cell.row * m_cellSize;

        ImVec2 p_min(x, y);
        ImVec2 p_max(x + m_cellSize, y + m_cellSize);

        bool hovered =
            mousePos.x >= p_min.x && mousePos.x < p_max.x &&
            mousePos.y >= p_min.y && mousePos.y < p_max.y;

        if (hovered)
        {
            m_hoveredCol = cell.col;
            m_hoveredRow = cell.row;
            m_hoveredIndex = cell.gridIndex;

            draw_list->AddRectFilled(
                p_min,
                p_max,
                IM_COL32(255, 255, 0, 60)
            );
        }

        draw_list->AddRect(
            p_min,
            p_max,
            IM_COL32(200, 200, 200, 120)
        );
    }

}


// this is the tile 
void App::AddPlane(const glm::vec2& pos)
{

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

    LOG_INFO("Added Tile. Total entities: " << m_entities.size());

}

void App::AppShutdown()
{
    LOG_INFO("Shutting down application");
	m_entities.clear(); // clear entities to release resources (optional, as unique_ptr will handle this)
    glfwTerminate(); // single place to call glfwTerminate
}

