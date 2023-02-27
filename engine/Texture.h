#pragma once

// The texture class helps add details to an object.
// Loads images files with the stb_image image loader
// and saves image details within its member variables.
// All texture objects are referenced with an integer,
// and provides functions to help set "this" as the currently
// bound texture. Textures take in the file path of the image.
class Texture
{
public:
    Texture(const char *textureFile);
    ~Texture();

    void SetActive();

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