# Require CMake version 3.16
cmake_minimum_required(VERSION 3.16)

# Require C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Project name
project(GraphicsEngine)

# Disable GLFW tests, examples, and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

include_directories(include)

# Add root directory of GLFW source to project
add_subdirectory(glfw)
add_subdirectory(engine)


# Link engine with glfw
target_link_libraries(engine glfw)



