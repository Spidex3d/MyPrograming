#version 460 core

// Minimal fragment shader that outputs a textured color.

out vec4 FragColor;

in vec3 uColor;
in vec2 TexCoord;

uniform sampler2D myTexture; // sampler bound to texture unit 0 by default

void main()
{
    // Sample texture with the interpolated texture coordinates
    FragColor = texture(myTexture, TexCoord);
}
