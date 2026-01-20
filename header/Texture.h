#pragma once

#include <string>
#include <glad/glad.h>

class Texture {
public:
    // Load immediately from file. 
    Texture(const std::string& path);
    ~Texture();

    // Disallow copying (simple beginner-safe approach).
	Texture(const Texture&) = delete; // disallow copy constructor
	Texture& operator=(const Texture&) = delete; // disallow copy assignment

    // Simple helpers
   
	void Bind(unsigned int unit = 0) const; // bind texture to texture unit
	static void Unbind(unsigned int unit = 0); // unbind texture from texture unit

	bool IsLoaded() const { return tex_ID != 0; } // check if texture is loaded successfully
	GLuint ID() const { return tex_ID; }   // get OpenGL texture ID
    

private:
	GLuint tex_ID{ 0 }; // OpenGL texture ID is 0 if not loaded
    int width, height, nr_Channels;
};
