cmake_minimum_required(VERSION 3.10)
project(MiniProjet LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PYTHON            "Use PYTHON"         OFF)
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

add_subdirectory(external/libigl)

set(SrcFiles
        external/paillier/paillier.c
        external/paillier/paillier.h
        src/main.cpp
        src/chiffre32.cpp
        src/chiffre32.hpp
        src/rsa.hpp
        src/rsa.cpp
        src/paillier.cpp
        src/paillier.hpp src/chiffre64.cpp src/chiffre64.hpp src/encryption3d.hpp src/encryption3d.cpp src/encryption3d.hpp)

add_executable(MiniProjet ${SrcFiles})

target_include_directories(MiniProjet PRIVATE ./external)

target_link_libraries(MiniProjet igl::core igl::opengl_glfw gmp)
