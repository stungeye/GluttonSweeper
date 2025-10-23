#pragma once

#include "raylib.h"
#include <string>

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
