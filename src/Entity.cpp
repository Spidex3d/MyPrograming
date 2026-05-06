#include "../header/Entity.h"
#include "../header/log.h"
#include <iostream>
#include "../header/App.h"

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

    
	// if we reach here, initialization has succeeded
    initialized = true;
    return true;
}

void Entity::DrawGameObj(GameObj* obj)
{
    if (!initialized || !obj) return;

    shader->Use();

    if (texture && texture->IsLoaded()) {
        texture->Bind(0);
    }

    PlaneModel* plane = dynamic_cast<PlaneModel*>(obj);
    if (plane && obj->isVisible) {
        plane->DrawPlane();
    }

    Texture::Unbind(0);
    glUseProgram(0);

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
