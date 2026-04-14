#pragma once
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include "../header/shader.h"
#include "../header/Texture.h"

struct GameObj { // Any Editor object name change from gameObj

    virtual ~GameObj() = default; // make polymorphic for safe dynamic_cast

    GameObj()
        : entId(-1),
        entTypeID(-1),
        entObjectIndex(-1),
        entName(),
        position(0.0f),
        scale(1.0f),
        rotation(0.0f),
        modelMatrix(1.0f),
        isRotateY(false),
        isVisible(true),
        tex_ID(0),
        texPath(""),
        //entType(""),
        assetPath("")
    {
    }

    int entId;          // individual entity ID
    int entTypeID;      // type of entity ie; plane, cube, npc, pickup etc
    int entObjectIndex; // index to the model object in the model manager Ie; how many objects of this type exist
    std::string entName;

    glm::vec3 position;     // Position of the object
    glm::vec3 scale;        // Scale of the object
    glm::vec3 rotation;     // Rotation of the object
    glm::mat4 modelMatrix;  // Model matrix for transformations

    bool isRotateY;          // whether the entity should rotate around the Y-axis (for simple animation)
    bool isVisible;         // Render or not
    // Textures for game objects like cubes and planes,
    unsigned int tex_ID;
    std::string texPath; // path to the texture file, used for loading and debugging, not used at runtime after texture is loaded
    // used for complex models that have their own materials/textures (like gltf/obj models) but can be applied to them as a single texture
    // if desired (e.g. for quick testing or if the model is untextured)

    //std::string entType;    // e.g. "Cube", "Plane", "Floor", "Obj", "Gltf", "Skybox"
    std::string assetPath;  // optional: path to model  "Obj", "Gltf", "Skybox" file used to create this entity

};
class Entity {
public:
    Entity();
    ~Entity();

    // Initialize shader, mesh, texture. Returns true on success.
    bool Initialize(GLFWwindow* window);

    void DrawGameObj(GameObj* obj);

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

const int OBJ_PLANE = 2; 


class PlaneModel : public GameObj {

public:
    GLuint VAO = 0, VBO = 0, EBO = 0;

    PlaneModel(int idx, const std::string& name, int Planeobjidx) {
        entId = idx;
        entName = name;
        entObjectIndex = Planeobjidx;
        entTypeID = OBJ_PLANE; // from globalVar.h = 2

        // default transform
        position = glm::vec3(0.0f);
        scale = glm::vec3(1.0f);
        rotation = glm::vec3(0.0f);
        modelMatrix = glm::mat4(1.0f);


        float vertices[] = {
            //Positions          Normals          Tex coords
             0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,
             0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,
            -0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f, 1.0f
        };
        unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        // Normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        // Texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    ~PlaneModel() {
        if (VAO) { glDeleteVertexArrays(1, &VAO); VAO = 0; }
        if (VBO) { glDeleteBuffers(1, &VBO); VBO = 0; }
        if (EBO) { glDeleteBuffers(1, &EBO); EBO = 0; }
    }

    void DrawPlane() {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // using indices
        glBindVertexArray(0);
    }

private:

};


