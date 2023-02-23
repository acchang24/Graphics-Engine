// Specify OpenGL 4.2 with core functionality
#version 420 core

layout (location = 0) in vec3 position; // position variable has attribute position 0

// Specify a vec4 color output to the fragment shader
out vec4 vertexColor;

void main()
{
    gl_Position = vec4(position, 1.0f); // Directly give a vec3 to vec4 constructor
    vertexColor = vec4(0.2f, 0.5f, 1.0f, 1.0f);// set the output variable
}