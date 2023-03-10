#pragma once
#include <string>
#include <unordered_map>

class AssetManager;

// Cache is a template class used by the AssetManager.
// It helps store assets into its respective templated map
// for on demand storing and loading.
template <class T>
class Cache
{
public:
    Cache(AssetManager *manager) : mManager(manager)
    {
    }

    ~Cache()
    {
        // Calls Clear()
        Clear();
    }

    //   StoreCache takes in a key value pair that stores
    //   into the templated asset's asset map
    // - const std::string& for the key name
    // - T* for the templated asset
    void StoreCache(const std::string &key, T *asset)
    {
        if (mAssetMap.find(key) == mAssetMap.end())
        {
            mAssetMap[key] = asset;
        }
    }

    //   Get() returns a templated cached asset by name
    // - const std::string& for the asset's key
    T *Get(const std::string &name)
    {
        if (mAssetMap.find(name) != mAssetMap.end())
        {
            return mAssetMap[name];
        }
        return nullptr;
    }

    // Clears each element in the asset map
    void Clear()
    {
        for (auto a : mAssetMap)
        {
            delete a.second;
        }
        mAssetMap.clear();
    }

private:
    // Pointer to a static AssetManager
    AssetManager *mManager;

    // Unordered map for the cached assets
    std::unordered_map<std::string, T *> mAssetMap;
};
