// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position; 

// texture variable has attribute position 1
layout (location = 1) in vec2 texCoord;

// Specify a vec2 texture output to the fragment shader
out vec2 textureCoord;

uniform mat4 transform;

uniform mat4 model;

uniform mat4 viewProj;

void main()
{
    // Directly give a vec3 to vec4 constructor
    // Multiply by transform matrix
    gl_Position = viewProj * model * transform * vec4(position, 1.0f); 
    
    textureCoord = texCoord;
}