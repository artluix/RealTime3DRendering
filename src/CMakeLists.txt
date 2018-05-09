cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(RealTime3DRendering)

add_subdirectory(externals)
add_subdirectory(library)
add_subdirectory(game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
