cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

if(APPLICATION)
    project(RealTime3DRendering VERSION 0.1)
elseif(TOOLS)
    project(RealTime3DRendering_Tools VERSION 0.1)
endif()

set(DIST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../dist)
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tools)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

set(CMAKE_DEBUG_POSTFIX "_d")

add_subdirectory(externals)
add_subdirectory(library)
add_subdirectory(game)

if(APPLICATION)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT game)
endif()
