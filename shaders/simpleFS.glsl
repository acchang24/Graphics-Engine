// Specify OpenGL 4.2 with core functionality
#version 420 core

// Input variables from vertex shader (same name and same type as in vertex shader so it is linked to variable in vertex shader)
in vec4 vertexColor; 
in vec2 textureCoord;

// Set samplers for 2d textures as a uniform
uniform sampler2D textureSampler;
uniform sampler2D textureSampler2;

// uniform vec4
uniform vec4 changeColor;

// Final vector4 pixel color output
out vec4 fragColor; 

void main()
{
    // Combine two textures in same place with mix function
    // Takes two values as input and lerps between based on float value 0.0 - 1.0
    fragColor = mix(texture(textureSampler, textureCoord), texture(textureSampler2, textureCoord), 0.3);// * vertexColor;

    // Sample colors of a texture with texture function:
    // - Takes in a texture sampler and a texture coordinate
    // fragColor = texture(textureSampler, textureCoord) * vertexColor;

    //fragColor = vertexColor;
}