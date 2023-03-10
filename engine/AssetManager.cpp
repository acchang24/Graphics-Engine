#include "AssetManager.h"
#include <iostream>

AssetManager *AssetManager::sManager = nullptr;

AssetManager::AssetManager()
{
    if (sManager)
    {
        std::cout << "There can only be one asset manager" << std::endl;
    }
    else
    {
        sManager = this;
        mShaderCache = new Cache<Shader>(this);
        mTextureCache = new Cache<Texture>(this);
    }
}

AssetManager::~AssetManager()
{
    std::cout << "Delete asset manager" << std::endl;
    sManager = nullptr;
    Clear();
}

void AssetManager::Clear()
{
    mShaderCache->Clear();
    mTextureCache->Clear();
}
