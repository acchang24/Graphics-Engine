#pragma once

class Texture
{
public:
    Texture(const char *textureFile);
    ~Texture();

    void SetActive();

private:
    // Int to store the texture reference id
    unsigned int mTextureID;

    // Width and height of the texture
    int mWidth;
    int mHeight;
    
    // Number of color channels
    int mNumChannels;
};