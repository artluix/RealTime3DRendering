cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(demo VERSION 0.1)

    set(DEMO_FILES
        # src
        src/Program.cpp
        src/Application.h
        src/Application.cpp

        # demo
        src/CubeComponent.h
        src/CubeComponent.cpp

        src/TriangleComponent.h
        src/TriangleComponent.cpp

        src/ModelComponent.h
        src/ModelComponent.cpp

        src/TextureModelComponent.h
        src/TextureModelComponent.cpp
    )

    add_executable(demo WIN32 ${DEMO_FILES})

    target_link_libraries(demo library)

    target_compile_options(demo
        PUBLIC
            /GR- # disable rtti
            /MP
            /std:c++17
    )

    target_compile_definitions(demo
        PUBLIC
            -DUNICODE
            -D_UNICODE

            # debug
            $<$<CONFIG:DEBUG>: -DDEBUG -D_DEBUG>
            # release
    )

    # properties
    set_target_properties(demo PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
    set_target_properties(demo PROPERTIES LINK_FLAGS_DEBUG /DEBUG:FASTLINK)

    # ide filters
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DEMO_FILES})

    # change target path instead
    set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR})
    set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR})

endif()