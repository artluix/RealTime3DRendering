cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_FILES
        # include
        include/library/Application.h
        include/library/Camera.h
        include/library/Clock.h
        include/library/Common.h
        include/library/Component.h
        include/library/DrawableComponent.h
        include/library/Exception.h
        include/library/FpsComponent.h
        include/library/NonCopyable.hpp
        include/library/RTTI.hpp
        include/library/Time.h
        include/library/ViewedComponent.h
        # src
        src/Application.cpp
        src/Clock.cpp
        src/Component.cpp
        src/DrawableComponent.cpp
        src/Exception.cpp
        src/FpsComponent.cpp
        src/Time.cpp
        src/ViewedComponent.cpp
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

    target_link_libraries(library d3d11 d3dcompiler dxguid dinput8)

    target_compile_options(library
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
    set_target_properties(library PROPERTIES
        DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX}
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