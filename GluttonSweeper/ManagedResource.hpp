#pragma once

#include "raylib.h"
#include <string>

// Traits to specialize resource loading/unloading for different raylib types
template <typename ResourceType>
struct ResourceTraits;

// Specialization for Texture2D
template <>
struct ResourceTraits<Texture2D> {
    static Texture2D Load(const char* path) { return LoadTexture(path); }
    static void Unload(Texture2D resource) { UnloadTexture(resource); }
    static bool IsValid(const Texture2D& resource) { return resource.id != 0; }
};

// Specialization for Font
template <>
struct ResourceTraits<Font> {
    static Font Load(const char* path) { return LoadFont(path); }
    static void Unload(Font resource) { UnloadFont(resource); }
    static bool IsValid(const Font& resource) { return resource.texture.id != 0; }
};

// Specialization for Sound
template <>
struct ResourceTraits<Sound> {
    static Sound Load(const char* path) { return LoadSound(path); }
    static void Unload(Sound resource) { UnloadSound(resource); }
    static bool IsValid(const Sound& resource) { return resource.frameCount > 0; }
};

// Specialization for Music
template <>
struct ResourceTraits<Music> {
    static Music Load(const char* path) { return LoadMusicStream(path); }
    static void Unload(Music resource) { UnloadMusicStream(resource); }
    static bool IsValid(const Music& resource) { return resource.frameCount > 0; }
};

// Generic RAII wrapper for raylib resources
//
// ManagedResource ensures that raylib resources are properly
// unloaded when the object is destroyed, preventing memory leaks.
//
// **IMPORTANT: Do not create a ManagedResource directly.**
// Use ResourceManager<ResourceType>::GetOrLoad() instead, which provides:
//   - Automatic caching (same file loaded only once)
//   - Shared ownership via std::shared_ptr
//   - Automatic cleanup when all references are dropped
//
// ManagedResource is non-copyable and non-movable because:
//   - Each instance owns a unique resource
//   - ResourceManager handles sharing via std::shared_ptr
//   - Moving/copying would complicate resource ownership
template <typename ResourceType>
class ManagedResource final {
public:
    ManagedResource(const std::string& path)
        : resource_(ResourceTraits<ResourceType>::Load(path.c_str())) {
    }

    // Disable copy and move constructors
    ManagedResource(const ManagedResource&) = delete;
    ManagedResource& operator=(const ManagedResource&) = delete;
    ManagedResource(ManagedResource&&) = delete;
    ManagedResource& operator=(ManagedResource&&) = delete;

    ~ManagedResource() {
        if (ResourceTraits<ResourceType>::IsValid(resource_)) {
            ResourceTraits<ResourceType>::Unload(resource_);
        }
    }

    bool isValid() const { 
        return ResourceTraits<ResourceType>::IsValid(resource_); 
    }
    
    // Implicit conversion for seamless use with raylib functions
    operator const ResourceType&() const { 
        return resource_; 
    }
    
    // Explicit access if needed (for backwards compatibility)
    const ResourceType& raw() const {
        return resource_;
    }

private:
    ResourceType resource_{};
};

// Specialization for Texture2D to add width() and height() helpers
template <>
class ManagedResource<Texture2D> final {
public:
    ManagedResource(const std::string& path)
        : resource_(ResourceTraits<Texture2D>::Load(path.c_str())) {
    }

    ManagedResource(const ManagedResource&) = delete;
    ManagedResource& operator=(const ManagedResource&) = delete;
    ManagedResource(ManagedResource&&) = delete;
    ManagedResource& operator=(ManagedResource&&) = delete;

    ~ManagedResource() {
        if (ResourceTraits<Texture2D>::IsValid(resource_)) {
            ResourceTraits<Texture2D>::Unload(resource_);
        }
    }

    bool isValid() const { 
        return ResourceTraits<Texture2D>::IsValid(resource_); 
    }
    
    // Implicit conversion for seamless use with raylib functions
    operator const Texture2D&() const { 
        return resource_; 
    }
    
    // Explicit access if needed (for backwards compatibility)
    const Texture2D& raw() const {
        return resource_;
    }
    
    // Texture-specific helpers
    int width() const { return resource_.width; }
    int height() const { return resource_.height; }

private:
    Texture2D resource_{};
};

// Type aliases for convenience
using ManagedTexture = ManagedResource<Texture2D>;
using ManagedFont = ManagedResource<Font>;
using ManagedSound = ManagedResource<Sound>;
using ManagedMusic = ManagedResource<Music>;
