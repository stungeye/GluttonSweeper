#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include "ManagedTexture.hpp"

// Manages texture loading and caching for the game.
//
// The TextureManager ensures each texture file is loaded only once
// and shared across all users via std::shared_ptr. 
//
// Key features:
//   - Automatic caching: Repeated loads of the same path return the same texture
//   - Shared ownership: Multiple objects can safely hold references to textures
//   - Automatic cleanup: Textures are unloaded when no longer referenced
//   - Thread-safety note: Must be used from the main thread (raylib requirement)
//
// Usage pattern:
//   1. Create a TextureManager (typically in main)
//   2. Call GetOrLoad(path) to get a Handle to a texture
//   3. Store the Handle as a member variable of type TextureManager::Handle
//   4. Access the raw Texture2D via handle->raw()
//   5. Textures are automatically unloaded when all Handles are destroyed
//
// Example in a game object:
//   class Player {
//       TextureManager::Handle spriteTexture;
//   public:
//       Player(TextureManager& tm, const std::string& filePath) {
//           spriteTexture = tm.GetOrLoad(filePath);
//       }
//       
//       void Draw() const {
//           if (spriteTexture && spriteTexture->isValid()) {  // Check if texture loaded successfully
//               DrawTexture(spriteTexture->raw(), x, y, WHITE);
//           }
//       }
//   };
//
// Example in GameplayScreen:
//   GameplayScreen::GameplayScreen(ScreenManager& manager) 
//       : FullScreen{ manager } {
//       playerTexture = GetContext().textureManager.GetOrLoad("Assets/player.png");
//   }
class TextureManager {
public:
    using Handle = std::shared_ptr<const ManagedTexture>;  // read-only to callers

    ~TextureManager() { unloadAll(); }

    // Load if missing; otherwise return the existing shared texture
    Handle GetOrLoad(const std::string& path) {
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
