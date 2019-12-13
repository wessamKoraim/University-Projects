cmake_minimum_required (VERSION 2.8.11)

# Set version information in a config.h file
set(game_VERSION_MAJOR 1)
set(game_VERSION_MINOR 0)
set(PROJECT_BINARY_DIR ${CMAKE_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR})
#set(BOX2D_DIR "${CMAKE_SOURCE_DIR}/liquidfun/liquidfun/Box2D")
#set(BOX2D_LIBRARY "${BOX2D_DIR}/Box2D/Release/libliquidfun.a")
set(BOX2D_INCLUDE_DIR "/usr/include/Box2D")

configure_file(
  "${PROJECT_SOURCE_DIR}/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  )
include_directories("${PROJECT_BINARY_DIR}")

# Enable debug symbols by default
# must be done before project() statement
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()
# (you can also set it on the command line: -D CMAKE_BUILD_TYPE=Release)

project (game)

set(EXECUTABLE_NAME "game")
add_executable(${EXECUTABLE_NAME} main.cpp map.cpp)


# Detect and add SFML
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
#Find any version 2.X of SFML
#See the FindSFML.cmake file for additional details and instructions
find_package(SFML 2 REQUIRED network audio graphics window system)
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

# Install target
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


# CPack packaging
#include(InstallRequiredSystemLibraries)
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/COPYING")
#set(CPACK_PACKAGE_VERSION_MAJOR "${game_VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR "${game_VERSION_MINOR}")
#include(CPack)

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})
  # or: target_link_libraries(${EXECUTABLE_NAME} ${OPENGL_gl_LIBRARY})
  target_link_libraries(${EXECUTABLE_NAME} m)  # if you use maths.h
endif()

# Box2D
#add_subdirectory("${BOX2D_DIR}" Box2D)
#include("${BOX2D_DIR}/Box2D/UseBox2D.cmake")
find_package(Box2D REQUIRED)
#include_directories(${BOX2D_DIR})
target_link_libraries(game Box2D)

