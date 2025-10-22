# Install script for directory: C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/src/5.5-966575b391.clean/src

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/pkgs/raylib_x64-windows")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "OFF")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/x64-windows-rel/raylib/raylib.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE SHARED_LIBRARY FILES "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/x64-windows-rel/raylib/raylib.dll")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/src/5.5-966575b391.clean/src/raylib.h"
    "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/src/5.5-966575b391.clean/src/rlgl.h"
    "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/src/5.5-966575b391.clean/src/raymath.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/x64-windows-rel/raylib/raylib.pc")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/raylib" TYPE FILE FILES "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/x64-windows-rel/raylib/raylib-config-version.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/raylib" TYPE FILE FILES "C:/Users/kgeske/source/repos/GluttonSweeper/vcpkg_installed/x64-windows/vcpkg/blds/raylib/src/5.5-966575b391.clean/src/../cmake/raylib-config.cmake")
endif()

