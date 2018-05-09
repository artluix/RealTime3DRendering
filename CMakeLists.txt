cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(RealTime3DRendering VERSION 0.1)

set(DIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dist)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

add_subdirectory(src)