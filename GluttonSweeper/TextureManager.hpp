#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ManagedTexture.hpp"

class TextureManager {
public:
    using Handle = std::shared_ptr<const ManagedTexture>;  // read-only to callers

    ~TextureManager() { unloadAll(); }

    // Load if missing; otherwise return the existing shared texture
    Handle getOrLoad(const std::string& path) {
        if (auto it = textureCache.find(path); it != textureCache.end()) {
			TraceLog(LOG_INFO, "TextureManager: Reusing cached texture: %s", path.c_str());
            return it->second;
        }

        auto tex = std::make_shared<ManagedTexture>(path);   // call on main thread, after InitWindow()
        if (!tex->isValid()) return nullptr;          // failed to load; do NOT insert
        textureCache.emplace(path, tex);
        return tex;
    }

    // Drop all references; Texture destructors call UnloadTexture()
    void unloadAll() {
        textureCache.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<ManagedTexture>> textureCache;
};
