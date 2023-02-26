// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position; 

// color variable has attribute position 1
layout (location = 1) in vec4 color; 

// texture variable has attribute position 2
layout (location = 2) in vec2 texCoord;

// Specify a vec4 color output to the fragment shader
out vec4 vertexColor;
// Specify a vec2 texture output to the fragment shader
out vec2 textureCoord;

void main()
{
    // Directly give a vec3 to vec4 constructor
    gl_Position = vec4(position, 1.0f); 

    // set the output variable to the input color from the vertex data
    vertexColor = color;
    
    textureCoord = texCoord;
}