#version 460 core

// Simple vertex shader that accepts a vec3 position (location = 0)
// and outputs clip-space position.
layout(location = 0) in vec3 aPosition; // The vertex position
layout(location = 1) in vec3 aColor;  // color or normal
layout(location = 2) in vec2 aTexCoord; // texture coordinates, UV

// names now match fragment shader inputs
out vec3 uColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPosition, 1.0);
    uColor = aColor;
    TexCoord = aTexCoord;
}