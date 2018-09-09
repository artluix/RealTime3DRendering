cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_FILES
        # include
        include/library/Application.h
        include/library/Camera.h
        include/library/Stopwatch.h
        include/library/Common.h
        include/library/Component.h
        include/library/DrawableComponent.h
        include/library/Exception.h
        include/library/FpsComponent.h
        include/library/Key.h
        include/library/KeyboardComponent.h
        include/library/MouseComponent.h
        include/library/NonCopyable.hpp
        include/library/Path.h
        include/library/RTTI.hpp
        include/library/Time.h
        include/library/ViewedComponent.h
        include/library/Utils.h
        # src
        src/Application.cpp
        src/Stopwatch.cpp
        src/Component.cpp
        src/DrawableComponent.cpp
        src/Exception.cpp
        src/FpsComponent.cpp
        src/KeyboardComponent.cpp
        src/MouseComponent.cpp
        src/Path.cpp
        src/ViewedComponent.cpp
        src/Utils.cpp
    )

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
            $<TARGET_PROPERTY:directxtk,INTERFACE_INCLUDE_DIRECTORIES>
            $<TARGET_PROPERTY:effects11,INTERFACE_INCLUDE_DIRECTORIES>
        PRIVATE
            src
    )

    target_link_libraries(library d3d11 d3dcompiler dxguid dinput8)

    target_compile_options(library
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

    # ide filters
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBRARY_FILES})

    # install
    install(TARGETS library
        LIBRARY DESTINATION ${DIST_DIR}/library/lib
        ARCHIVE DESTINATION ${DIST_DIR}/library/lib
    )
    install(DIRECTORY include DESTINATION ${DIST_DIR}/library)
endif()