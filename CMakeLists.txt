# ====================================================
#  Project: GLUTApp UTS
#  Author : Yohanes Oktanio
#  NIM    : 24-095
#  Website: yohanes.oktanio.dev
# ====================================================
#  Deskripsi:
#  CMake configuration file for building an
#  OpenGL + GLUT based graphics project (multiple targets).
#  Message : Mesakno Developere Mas/Mbak. Jajal Tukokno Maem
# ====================================================

cmake_minimum_required(VERSION 3.10)
project(GLUTApp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(main main.cpp)

foreach(target main)

    target_link_libraries(${target} PRIVATE 
        OpenGL::GL
        OpenGL::GLU
        GLUT::GLUT
    )
endforeach()