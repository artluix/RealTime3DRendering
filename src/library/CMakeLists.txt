cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_FILES
        # include
        include/library/Application.h
        include/library/Stopwatch.h
        include/library/Color.h
        include/library/Common.h

        include/library/Components/BaseComponent.h
        include/library/Components/CameraComponent.h
        include/library/Components/CubeDemoComponent.h
        include/library/Components/DrawableComponent.h
        include/library/Components/FirstPersonCameraComponent.h
        include/library/Components/FpsComponent.h
        include/library/Components/GridComponent.h
        include/library/Components/KeyboardComponent.h
        include/library/Components/MouseComponent.h
        include/library/Components/SceneComponent.h
        include/library/Components/TextComponent.h
        include/library/Components/TriangleDemoComponent.h
        include/library/Components/UIComponent.h

        include/library/Exception.h
        include/library/Key.h
        include/library/Logger.h
        include/library/Math.h
        include/library/Matrix.h
        include/library/NonCopyable.hpp
        include/library/NonConstructible.hpp
        include/library/Path.h
        include/library/RTTI.hpp
        include/library/RenderStatesStorage.h
        include/library/ServiceContainer.h
        include/library/Time.h
        include/library/Utils.h
        include/library/Vector.h
        include/library/VertexTypes.h

        # src
        src/Application.cpp

        src/Components/BaseComponent.cpp
        src/Components/CameraComponent.cpp
        src/Components/CubeDemoComponent.cpp
        src/Components/DrawableComponent.cpp
        src/Components/FirstPersonCameraComponent.cpp
        src/Components/FpsComponent.cpp
        src/Components/GridComponent.cpp
        src/Components/KeyboardComponent.cpp
        src/Components/MouseComponent.cpp
        src/Components/SceneComponent.cpp
        src/Components/TextComponent.cpp
        src/Components/TriangleDemoComponent.cpp
        src/Components/UIComponent.cpp

        src/Exception.cpp
        src/Logger.cpp
        src/Matrix.cpp
        src/Path.cpp
        src/RenderStatesStorage.cpp
        src/ServiceContainer.cpp
        src/Stopwatch.cpp
        src/Utils.cpp
        src/Vector.cpp
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
            /std:c++17
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