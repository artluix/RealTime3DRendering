cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(RealTime3DRendering VERSION 0.1)
elseif(TOOLS)
    project(RealTime3DRendering_Tools VERSION 0.1)
endif()

# set platform bools (works only after first project command)
set(IS_X64 FALSE)
set(IS_WIN32 FALSE)

if(CMAKE_VS_PLATFORM_NAME STREQUAL "x64")
    set(IS_X64 TRUE)
elseif(CMAKE_VS_PLATFORM_NAME STREQUAL "Win32")
    set(IS_WIN32 TRUE)
endif()

message("Platform: " ${CMAKE_VS_PLATFORM_NAME})
message("Platform Win32: " ${IS_WIN32})
message("Platform x64: " ${IS_X64})

set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../bin)
set(TOOLS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../tools)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

# reduce config types to only Debug and Release
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)

set(CMAKE_DEBUG_POSTFIX "_d") # set global debug postfix for all libraries

add_subdirectory(externals)
add_subdirectory(library)
add_subdirectory(demo)

if(APPLICATION)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT demo)
endif()
