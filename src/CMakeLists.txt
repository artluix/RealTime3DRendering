cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(RealTime3DRendering VERSION 0.1)

set(DIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(externals)
add_subdirectory(library)
add_subdirectory(game)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
