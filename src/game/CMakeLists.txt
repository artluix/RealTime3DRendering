cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    set(INSTALL_GAME FALSE)

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
            /MP
    )

    target_compile_definitions(library
        PUBLIC
            -DUNICODE
            -D_UNICODE

            # debug
            $<$<CONFIG:DEBUG>: -DDEBUG -D_DEBUG>
            # release
    )

    # set postfix
    set_target_properties(game PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

    set_target_properties(game PROPERTIES WIN32_EXECUTABLE TRUE)

    # ide filters
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GAME_FILES})

    message(STATUS "Install game: " ${INSTALL_GAME})

    if (${INSTALL_GAME})
        install(TARGETS game RUNTIME DESTINATION ${BIN_DIR})
    else()
        # change target path instead
        set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})
        set_target_properties(game PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})
    endif()

endif()