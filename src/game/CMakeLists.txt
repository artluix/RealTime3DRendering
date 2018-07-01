cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

project(game VERSION 0.1)

set(GAME_FILES
    # src
    src/Program.cpp
    src/Game.h
    src/Game.cpp
)

add_executable(game
    ${GAME_FILES}
)

target_link_libraries(game library)

target_compile_options(game
    PUBLIC
        /GR- # disable rtti
)

target_compile_definitions(library
    PUBLIC
        -DUNICODE
        -D_UNICODE
        # debug
        $<$<CONFIG:DEBUG>: -DDEBUG -D_DEBUG>
        # release
        $<$<CONFIG:RELEASE>: -UDEBUG -U_DEBUG>
)

# add postfix
set_target_properties(game PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
set_target_properties(game PROPERTIES WIN32_EXECUTABLE TRUE)

# ide filters
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_FILES})

# install
install(TARGETS game
    RUNTIME DESTINATION ${BIN_DIR}
)
