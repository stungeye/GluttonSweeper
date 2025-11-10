#pragma once

struct DisplayInfo {
    int monitorIndex;
    int monitorWidth;
    int monitorHeight;
};

struct ViewportPx {
    int width;
    int height;
};

// Provides window/display information, abstracting raylib queries
// This keeps screens from directly querying monitor/window state
class DisplayService {
public:
    // Get current display information
    DisplayInfo GetInfo() const;
    
    // Get available viewport (monitor size)
    ViewportPx GetViewportPx() const;
};
