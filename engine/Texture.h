#pragma once

// The Texture class helps add details to an object.
// Loads images files with the stb_image image loader
// and saves image details within its member variables.
// All texture objects are referenced with an integer,
// and provides functions to help set "this" as the currently
// bound texture.
class Texture
{
public:
    //   Texture constructor:
    // - const char* as the name/file path of the texture file
    Texture(const char *textureFile);
    ~Texture();

    // Binds the texure as the current texture using glBindTexture
    // Passes in GL_TEXTURE_2D and the texture ID as its paramaters
    void SetActive();

    // Getter for the texture's ID
    unsigned int GetID() { return mTextureID; }

private:
    // Int to store the texture reference id
    unsigned int mTextureID;

    // Width and height of the texture
    int mWidth;
    int mHeight;

    // Number of color channels
    int mNumChannels;
};