#pragma once
#include "glad\glad.h"
#include "GLFW\glfw3.h"
#include <vector>
#include <memory>

#include "Entity.h"
#include "Texture.h"
#include <imgui/imgui.h>

struct GridCell {
	int row =0;
	int col = 0;
	int gridIndex = 0; // index to the grid object in the model manager
	glm::vec2 worldPos = glm::vec2(0.0f); // world position of the cell center, used for placing objects on the grid
	int tileID = -1; // optional: ID of the tile occupying this cell, -1 if empty
	int entityIndex = -1; // optional: ID of the entity occupying this cell, -1 if empty
    bool isOccupied = false;
};

constexpr const char* TILE_PATH = "textures/Grass_01.png"; // This is just temp


class App {
public:
    

    // Returns a pointer to a single App instance (simple singleton).
    static App* Instance();
	// Set the callback for UI actions (e.g. menu selections) add all game objects
    void SetActionCallback(const std::string& cmd);
    // Run the application (initializes subsystems, enters render loop).
    int RunApp();
	// Editor grid creation and validation and draw
	void AddGrid(int rows, int cols, float cellSize);
   
    // void DrawGrid(GridCell* cell, float cellSize, ImVec2 m_imagePos); // draw the grid lines 
    void DrawGrid(ImVec2 m_imagePos);

    void AddTileAtHoveredCell();
    void DeleteSelectedEntity();

	// add a plane to the scene at the given position (default is origin) later we can put it in grid cells and snap to grid etc
    void AddPlane(const glm::vec2& pos = glm::vec2(0.0f));

    // Shutdown and cleanup (called automatically from RunApp before exit).
    void AppShutdown();


private:
    App() = default;
    ~App() = default;

    // non-copyable
    App(const App&) = delete;
    App& operator=(const App&) = delete;

    int m_selectedEntityIndex = -1;

    // Editor Grid
	int m_rows = 10;  // set up grid size 
	int m_cols = 12;
	float m_cellSize = 64.0f; // set up cell size as needed

    int m_gridIndex = -1;
    int m_gridRow = -1;
    int m_gridCol = -1;

    int m_hoveredRow = -1;
    int m_hoveredCol = -1;
    int m_hoveredIndex = -1;
   
   
	// this code dose what? it creates a unique pointer to a Texture object and initializes it to nullptr.
    std::unique_ptr<Texture> m_tileTexture = nullptr;
    GLuint m_tileTextureID = 0;
    

	std::vector<GridCell> m_grid;

    //std::unique_ptr<App> m_entity;
    std::vector<std::unique_ptr<GameObj>> m_entities;

};

