#pragma once
#include "Cache.h"
#include "Shader.h"
#include "Texture.h"

// The AssetManager is a singleton class that helps load assets on demand
// and cache them so that subsequent loads will return the cached asset
// instead of having to load them again. This manager provides functions
// to help save/load assets on when needed.
class AssetManager
{
public:
    AssetManager();
    ~AssetManager();

    // Clear() goes to each Caches and deletes them
    void Clear();

    // Returns the static AssetManager
    static AssetManager *Get() { return sManager; }

    // Save/load for shader cache
    void SaveShader(const std::string &shaderName, Shader *shader) { mShaderCache->StoreCache(shaderName, shader); }
    Shader *LoadShader(const std::string &shaderName) { return mShaderCache->Get(shaderName); }

    // Save/load for texture cache
    void SaveTexture(const std::string &textureName, Texture *texture) { mTextureCache->StoreCache(textureName, texture); }
    Texture *LoadTexture(const std::string &textureName) { return mTextureCache->Get(textureName); }

private:
    // Singleton
    static AssetManager *sManager;

    // Shader cache
    Cache<Shader> *mShaderCache;

    // Texture cache
    Cache<Texture> *mTextureCache;
};
