cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(library VERSION 0.1)

set(LIBRARY_FILES
    # include
    include/library/Library.h
    # src
    src/Library.cpp
)

# PTODO: Add headers here because IDE can't recognize them
add_library(library
    STATIC
        $<TARGET_OBJECTS:directxtk>
        $<TARGET_OBJECTS:effects11>
    ${LIBRARY_FILES}
)

target_include_directories(library
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)

# ide filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBRARY_FILES})

# install
install(TARGETS library
    LIBRARY DESTINATION ${DIST_DIR}/library/lib
    ARCHIVE DESTINATION ${DIST_DIR}/library/lib
)
install(DIRECTORY include DESTINATION ${DIST_DIR}/library)
