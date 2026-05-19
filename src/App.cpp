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

        AddTileAtHoveredCell();

        /*if (m_hoveredIndex >= 0 && m_hoveredIndex < m_grid.size())
        {
            m_grid[m_hoveredIndex].tileID = 1;
        }*/
    } // Now go to line 64 App.cpp
   
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
            GameObj* obj = m_entities[m_selectedEntityIndex].get();

            if (obj) {
                        // Name / rename
                        char nameBuf[128];
                        //strncpy(nameBuf, selected->entName.c_str(), sizeof(nameBuf));
                        strncpy_s(nameBuf, obj->entName.c_str(), sizeof(nameBuf));
                        nameBuf[sizeof(nameBuf) - 1] = '\0';
                        if (ImGui::InputText("Name", nameBuf, sizeof(nameBuf))) {
                            obj->entName = std::string(nameBuf);
                        }
            }

            if (m_selectedEntityIndex < 0 || m_selectedEntityIndex >= (int)m_entities.size())
            {
                ImGui::Text("No object selected.");
                ImGui::End();
                return 0;
            }

            if (!obj)
            {
                ImGui::Text("Invalid object.");
                ImGui::End();
                return 0;
            }

            if (TileObj* tile = dynamic_cast<TileObj*>(obj))
            {
           
            ImGui::SeparatorText("Tile Grid Info");
            ImGui::Text("Tile ID: %d", obj->entId);
			ImGui::Text("Tile Type ID: %d", obj->entTypeID); // Ground / Water etc
            ImGui::Text("Tile Row: %d", tile->row);
            ImGui::Text("Tile Col: %d", tile->col);
            ImGui::Text("Tile Grid Index: %d", tile->gridIndex);
            ImGui::Separator();
            ImGui::DragFloat3("Position", &obj->position.x, 1.0f);
            ImGui::DragFloat3("Scale", &obj->scale.x, 1.0f);
            ImGui::DragFloat("Width", &tile->width, 1.0f);
            ImGui::DragFloat("Height", &tile->height, 1.0f);
            
            ImGui::SeparatorText("Tile Properties...");               

			ImGui::Checkbox("Visible", &obj->isVisible); // toggle rendering of this tile in the render loop
			ImGui::Checkbox("Is Collectable", &tile->isCollectable); // can be collected by player for points or powerups etc
            ImGui::Checkbox("Has Collision", &tile->hasCollision);
			ImGui::Checkbox("Solid", &tile->isSolid); // Land / Water etc
                            
            }
           
            if (ImGui::Button("Delete")) {
                DeleteSelectedEntity();
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit")) {
                // close object inspector - editor
                m_selectedEntityIndex = -1;
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
               // std::string displayName = obj->entName.empty() ? ("Entity " + std::to_string(obj->entId)) : obj->entName;
               // std::string label = std::string(displayName + "##" + std::to_string(obj->entId));
                std::string label = std::string(obj->entName);
           

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
          
            if (cell.entityIndex == -1)
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
void App::AddTileAtHoveredCell()
{
    if (m_hoveredIndex < 0 || m_hoveredIndex >= (int)m_grid.size())
        return;

    GridCell& cell = m_grid[m_hoveredIndex];

    // Do not place another tile if this cell already has one
    if (cell.entityIndex != -1)
        return;

    int newId = static_cast<int>(m_entities.size());

   // std::string name = "Tile " + std::to_string(newId);
    std::string name = "Tile ";

    auto tile = std::make_unique<TileObj>(
        newId,
        name,
        cell.row,
        cell.col,
        cell.gridIndex,
        m_cellSize
    );

    cell.tileID = 1;
    cell.entityIndex = newId;

    m_entities.push_back(std::move(tile));

    LOG_INFO("Added tile object: " << name
        << " row: " << cell.row
        << " col: " << cell.col
        << " index: " << cell.gridIndex);
}
// Delete the currently selected entity - tile.
void App::DeleteSelectedEntity()
{
    if (m_selectedEntityIndex < 0 || m_selectedEntityIndex >= (int)m_entities.size())
        return;

    GameObj* obj = m_entities[m_selectedEntityIndex].get();
    if (!obj)
        return;

    // If this object is a tile, clear its grid cell first
    if (TileObj* tile = dynamic_cast<TileObj*>(obj))
    {
        if (tile->gridIndex >= 0 && tile->gridIndex < (int)m_grid.size())
        {
            GridCell& cell = m_grid[tile->gridIndex];

            cell.tileID = -1;
            cell.entityIndex = -1;

            LOG_INFO("Cleared grid cell: row "
                << cell.row
                << ", col "
                << cell.col
                << ", index "
                << cell.gridIndex);
        }
    }

    // Remove object from scene list
    m_entities.erase(m_entities.begin() + m_selectedEntityIndex);

    // Important: fix entityIndex references after erase
    for (auto& cell : m_grid)
    {
        if (cell.entityIndex > m_selectedEntityIndex)
        {
            cell.entityIndex--;
        }
    }

    m_selectedEntityIndex = -1;
}


// this was the tile 
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

