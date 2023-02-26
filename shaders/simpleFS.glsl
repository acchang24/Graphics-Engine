// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader (same name and same type as in vertex shader so it is linked to variable in vertex shader)
in vec4 vertexColor; 
in vec2 textureCoord;

// Set a sampler for 2d textures as a uniform
uniform sampler2D textureSampler;

// uniform vec4
uniform vec4 changeColor;

// Final vector4 pixel color output
out vec4 fragColor; 

void main()
{
    // Sample colors of a texture with texture function:
    // - Takes in a texture sampler and a texture coordinate
    fragColor = texture(textureSampler, textureCoord) * vertexColor;

    //fragColor = vertexColor;
}