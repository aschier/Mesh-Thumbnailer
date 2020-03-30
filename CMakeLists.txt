cmake_minimum_required(VERSION 3.1)
project(mesh_thumbnailer)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(MSVC_RUNTIME "dynamic")

option(LIBIGL_WITH_OPENGL            "Use OpenGL"         ON)
option(LIBIGL_WITH_OPENGL_GLFW       "Use GLFW"           ON)
option(LIBIGL_WITH_PNG               "Use PNG"            ON)

find_package(LIBIGL REQUIRED QUIET)

add_library(mesh_thumbnailer STATIC
	mesh_thumbnailer.cpp
)
target_link_libraries(mesh_thumbnailer
	igl::core igl::opengl_glfw igl::png
)
target_include_directories(mesh_thumbnailer
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(mesh_thumbnailer_bin
	programs/mesh_thumbnailer_bin.cpp
)
target_link_libraries(mesh_thumbnailer_bin
	mesh_thumbnailer
)
set_target_properties(mesh_thumbnailer_bin
	PROPERTIES OUTPUT_NAME "mesh_thumbnailer"
)
target_include_directories(mesh_thumbnailer_bin
	PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS mesh_thumbnailer_bin)
