// Specify OpenGL 4.2 with core functionality
#version 420 core

// position variable has attribute position 0
layout (location = 0) in vec3 position; 

// texture variable has attribute position 1
layout (location = 1) in vec2 texCoord;

// Uniforms for model to world
uniform mat4 model;

// viewProj takes both view and projection matrices
uniform mat4 viewProj;

// Specify a vec2 texture output to the fragment shader
out vec2 textureCoord;

void main()
{
    // Directly give a vec3 to vec4 constructor
    // Multiply by transform matrices
    gl_Position = viewProj * model * vec4(position, 1.0f); 
    
    textureCoord = texCoord;
}