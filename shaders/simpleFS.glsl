// Specify OpenGL 4.2 with core functionality
#version 420 core

// uniform vec4
uniform vec4 changeColor;

// Input variable from pixel shader (same name and same type as in vertex shader so it is linked to variable in vertex shader)
in vec4 vertexColor; 

// Final vector4 pixel color output
out vec4 fragColor; 

void main()
{
    fragColor = vertexColor;
}