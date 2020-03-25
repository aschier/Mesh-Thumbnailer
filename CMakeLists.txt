cmake_minimum_required(VERSION 3.1)
project(mesh_thumbnailer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(MSVC_RUNTIME "dynamic")

option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PNG               "Use PNG"            ON)

find_package(LIBIGL REQUIRED QUIET)

add_executable(mesh_thumbnailer
	mesh_thumbnailer.cpp
)
target_link_libraries(mesh_thumbnailer
	igl::core igl::opengl_glfw igl::png
)
