cmake_minimum_required(VERSION 3.10)
project(MiniProjet LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PYTHON            "Use PYTHON"         OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(libigl)

set(SrcFiles src/main.cpp
        src/encryption3D.cpp
        src/encryption3D.hpp
        src/rsa.hpp
        src/rsa.cpp)

add_executable(MiniProjet ${SrcFiles})
target_link_libraries(MiniProjet igl::core igl::opengl_glfw)
