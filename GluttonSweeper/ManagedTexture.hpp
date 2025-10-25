#pragma once

#include "raylib.h"
#include <string>

// RAII wrapper for raylib Texture2D resources.
//
// ManagedTexture ensures that textures loaded from disk are properly
// unloaded when the object is destroyed, preventing GPU memory leaks.
//
// **IMPORTANT: Do not create a ManagedTexture directly.**
//// Use TextureManager::GetOrLoad() instead, which provides:
////   - Automatic caching (same file loaded only once)
////   - Shared ownership via std::shared_ptr
////   - Automatic cleanup when all references are dropped
//
//// ManagedTexture is non-copyable and non-movable because:
////   - Each instance owns a unique GPU resource (Texture2D)
////   - TextureManager handles sharing via std::shared_ptr
////   - Moving/copying would complicate resource ownership
class ManagedTexture final {
public:
    ManagedTexture(const std::string& path)
        : tex_(LoadTexture(path.c_str())) {
    }

	// Disable copy and move constructors.
    ManagedTexture(const ManagedTexture&) = delete;
    ManagedTexture& operator=(const ManagedTexture&) = delete;
    ManagedTexture(ManagedTexture&&) = delete;
    ManagedTexture& operator=(ManagedTexture&&) = delete;

    ~ManagedTexture() {
        if (tex_.id != 0) UnloadTexture(tex_);
    }

    bool isValid()       const { return tex_.id != 0; }
    const Texture2D& raw() const { return tex_; }
    int  width()         const { return tex_.width; }
    int  height()        const { return tex_.height; }

private:
    Texture2D tex_{};  // GPU handle managed by raylib
};
