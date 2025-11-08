#pragma once

#include "ManagedResource.hpp"
#include <unordered_map>
#include <memory>
#include <string>

// Generic resource manager with caching for raylib resources.
//
// The ResourceManager ensures each resource file is loaded only once
// and shared across all users via std::shared_ptr.
//
// Key features:
//   - Automatic caching: Repeated loads of the same path return the same resource
//   - Shared ownership: Multiple objects can safely hold references to resources
//   - Automatic cleanup: Resources are unloaded when no longer referenced
//   - Thread-safety note: Must be used from the main thread (raylib requirement)
//
// Usage pattern:
//   1. Create a ResourceManager (typically in main)
//   2. Call GetOrLoad(path) to get a Handle to a resource
//   3. Store the Handle as a member variable of type ResourceManager::Handle
//   4. Use the resource directly with raylib functions (no ->raw() needed!)
//   5. Resources are automatically unloaded when all Handles are destroyed
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
//           if (spriteTexture && spriteTexture->isValid()) {
//               DrawTexture(spriteTexture, x, y, WHITE);  // No ->raw() needed!
//           }
//       }
//   };
template <typename ResourceType>
class ResourceManager {
public:
    using ManagedType = ManagedResource<ResourceType>;
    using Handle = std::shared_ptr<const ManagedType>;

    ~ResourceManager() { unloadAll(); }

    // Load if missing; otherwise return the existing shared resource
    Handle GetOrLoad(const std::string& path) {
        if (auto it = cache_.find(path); it != cache_.end()) {
            TraceLog(LOG_INFO, "ResourceManager: Reusing cached resource: %s", path.c_str());
            return it->second;
        }

        auto resource = std::make_shared<ManagedType>(path);
        if (!resource->isValid()) {
            TraceLog(LOG_WARNING, "ResourceManager: Failed to load: %s", path.c_str());
            return nullptr;
        }
        
        cache_.emplace(path, resource);
        TraceLog(LOG_INFO, "ResourceManager: Loaded new resource: %s", path.c_str());
        return resource;
    }

    // Drop all references; resource destructors handle cleanup
    void unloadAll() {
        cache_.clear();
    }

private:
    std::unordered_map<std::string, std::shared_ptr<ManagedType>> cache_;
};

// Type aliases for specific managers
using TextureManager = ResourceManager<Texture2D>;
using FontManager = ResourceManager<Font>;
using SoundManager = ResourceManager<Sound>;
using MusicManager = ResourceManager<Music>;
