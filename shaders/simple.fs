#version 420 core

// Input variable from pixel shader (same name and same type as in vertex shader so it is linked to variable in vertex shader)
in vec4 vertexColor;

// Final vector4 pixel color output
out vec4 FragColor;

void main()
{
    FragColor = vertexColor;
}