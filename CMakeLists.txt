cmake_minimum_required(VERSION 3.10)
project(Basic3DViewer VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Add include directory
include_directories(include)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW using pkg-config
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# Add executable
add_executable(Basic3DViewer src/main.cpp src/glad.c)

# Include GLFW directories
include_directories(${GLFW_INCLUDE_DIRS})

# Link libraries
target_link_libraries(Basic3DViewer ${GLFW_LIBRARIES} ${OPENGL_gl_LIBRARY} X11 pthread Xrandr Xi dl)
