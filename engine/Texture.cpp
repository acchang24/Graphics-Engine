#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

Texture::Texture(const char *textureFile)
    : mTextureID(0), mWidth(0), mHeight(0), mNumChannels(0)
{
    //   Create a texture object with glGenTextures:
    // - Takes in the number of textures to generate
    // - The unsigned int array to store the number of textures(can be a single uint)
    glGenTextures(1, &mTextureID);

    // Bind it to so any subsequent texture commands will use the currently bound texture
    // Binding after activating a texture unit will bind the texture to that unit
    // There is a minimum of 16 texture units to use (GL_TEXTURE0 to GL_TEXTURE15)
    glBindTexture(GL_TEXTURE_2D, mTextureID);

    // Set the texture's wrapping parameters (set on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture filtering parameters (set on currently bound texture)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load/Generate a texture
    // Tell stb_image.h to flip loaded textures on the y axis
    stbi_set_flip_vertically_on_load(true);

    // Load in texture file with stbi_load:
    // - Takes the location of the image file
    // - width, height, and number of color channels as ints
    unsigned char *data = stbi_load(textureFile, &mWidth, &mHeight, &mNumChannels, 0);

    if (data)
    {
        // Get the format based on the number of color channels
        GLenum format;
        if (mNumChannels == 3)
        {
            format = GL_RGB;
        }
        else if (mNumChannels == 4)
        {
            format = GL_RGBA;
        }

        //   Start generating a texture using the loaded image data
        //   Textures are generated with glTexImage2D:
        // - 1st argument specifies the texture target. Setting to GL_TEXTURE_2D
        //   will generate textures on the bound texture object at the same target
        // - 2nd argument specifies mipmap level to create a texture for. 0 is base level
        // - 3rd argument specifies the format to store the texture.
        // - 4th/5th arguments specifies width/height of the resulting texture
        // - 6th argument default to 6 (legacy)
        // - 7th/8th arguments specifies the format and datatype of the source image
        //   Loaded the image with RGB values, and stored them as chars(bytes)
        // - Last argument is the actual image data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, data);

        // Automatically generate all the required mipmaps for the currently bound texture
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    // Free image data
    stbi_image_free(data);
}

Texture::~Texture()
{
    std::cout << "Delete texture" << std::endl;
    // Delete the texture's resources
    glDeleteTextures(1, &mTextureID);
    mTextureID = 0;
    mWidth = 0;
    mHeight = 0;
    mNumChannels = 0;
}

void Texture::SetActive()
{
    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
