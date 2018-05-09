cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(game VERSION 0.1)

set(GAME_FILES
    # src
    src/Dummy.h
    src/Dummy.cpp
)

add_executable(game
    ${GAME_FILES}
)

target_link_libraries(game library)

# ide filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_FILES})

# install
install(TARGETS game
    RUNTIME DESTINATION ${BIN_DIR}
)
