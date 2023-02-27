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
};