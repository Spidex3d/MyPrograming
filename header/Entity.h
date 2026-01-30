#pragma once
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include "../header/shader.h"
#include "../header/Texture.h"

/*
  Simple Entity that owns:
   - a Shader program
   - a Texture
   - a mesh (VAO/VBO/EBO)
  Initialize() creates these once. Render() draws each frame.
*/
class Entity {
public:
    Entity();
    ~Entity();

    // Initialize shader, mesh, texture. Returns true on success.
    bool Initialize(GLFWwindow* window);

    // Render the entity (call each frame)
    void Render();

	// Non-copyable approach for simplicity and beginner safety
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

private:
    // resources
    
    Shader* shader = nullptr;   // pointer to allow simpler initialization ordering
	Texture* texture = nullptr; // pointer to allow simpler initialization ordering

	GLuint VAO = 0; // Vertex Array Object
	GLuint VBO = 0; // Vertex Buffer Object
	GLuint EBO = 0; // Element Buffer Object
	bool initialized = false; // track if initialized

    // helper to create mesh
    void SetupMesh();
	// helper to cleanup mesh
    void CleanupMesh();
};

