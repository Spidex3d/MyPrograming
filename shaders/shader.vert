#version 330 core

// Simple vertex shader that accepts a vec2 position (location = 0)
// and outputs clip-space position.
layout(location = 0) in vec2 aPosition;

void main()
{
    // Promote 2D input to vec4; Z = 0, W = 1
    gl_Position = vec4(aPosition, 0.0, 1.0);
}