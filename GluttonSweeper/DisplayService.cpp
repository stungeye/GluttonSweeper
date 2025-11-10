#include "DisplayService.hpp"
#include <raylib.h>

DisplayInfo DisplayService::GetInfo() const {
    DisplayInfo info{};
    info.monitorIndex = GetCurrentMonitor();
    info.monitorWidth = GetMonitorWidth(info.monitorIndex);
    info.monitorHeight = GetMonitorHeight(info.monitorIndex);
    return info;
}

ViewportPx DisplayService::GetViewportPx() const {
    const DisplayInfo info = GetInfo();
    return ViewportPx{ info.monitorWidth, info.monitorHeight };
}
