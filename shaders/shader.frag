#version 330 core

// Minimal fragment shader that outputs a solid color.

out vec4 FragColor;

uniform vec3 uColor; // optional uniform to change color from C++

void main()
{
    // Default orange-ish color if the uniform isn't changed
    FragColor = vec4(uColor, 1.0);
}