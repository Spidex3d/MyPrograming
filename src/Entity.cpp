#include "../header/Entity.h"
#include "../header/log.h"
#include <iostream>

Entity::Entity()
{
	// not used but here for completeness
}

Entity::~Entity()
{
    // delete heap-allocated resources if present
    if (shader) {
        delete shader;
        shader = nullptr;
    }
	// delete texture if present
    if (texture) {
        delete texture;
        texture = nullptr;
    }
    CleanupMesh();
}

bool Entity::Initialize(GLFWwindow* window)
{
    if (initialized) return true;

    // Create shader (keeps program alive for lifetime of Entity)
    shader = new Shader("shaders/shader.vert", "shaders/shader.frag");
    if (!shader) {
        LOG_ERROR("Failed to create Shader");
        return false;
    }
    shader->Use();
    shader->SetUniformInt("myTexture", 0); // bind sampler to texture unit 0 once

    // Load texture once
    texture = new Texture("textures/github.jpg");
    if (!texture->IsLoaded()) {
        LOG_WARNING("Failed to load texture");
        // continue anyway — shader will sample black/undefined but app not crash
    }

    // Setup mesh (VAO/VBO/EBO once)
    SetupMesh();
	// if we reach here, initialization has succeeded
    initialized = true;
    return true;
}

void Entity::SetupMesh()
{
    // Quad data (positions, placeholder normals/colors, texcoords)
    float vertices[] = {
        // Positions        Normals         TexCoords
         0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f,  1.0f, 1.0f,
         0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,  0.0f,0.0f,1.0f,  0.0f, 1.0f
    };
    GLuint indices[] = {
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

    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // Color/normal attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // TexCoord (location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // Unbind to leave clean state
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Entity::CleanupMesh()
{
    if (VAO) {
		glDeleteVertexArrays(1, &VAO); // delete VAO
        VAO = 0;
    }
    if (VBO) {
		glDeleteBuffers(1, &VBO); // delete VBO
        VBO = 0;
    }
    if (EBO) {
		glDeleteBuffers(1, &EBO); // delete EBO
        EBO = 0;
    }
}

void Entity::Render()
{
	// if not initialized, skip rendering
    if (!initialized) return;

    // Use shader and bind texture/unit then draw
    shader->Use();
    if (texture && texture->IsLoaded()) {
        texture->Bind(0);
    }

	// Bind VAO
    glBindVertexArray(VAO);
	// Draw the quad using element array on screen
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // optional unbind
    Texture::Unbind(0);
    glUseProgram(0);
}
