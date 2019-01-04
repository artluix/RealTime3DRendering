cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_SOURCES
        # include
        include/library/Application.h
        include/library/Stopwatch.h
        include/library/Color.h
        include/library/Common.h

        # components
        include/library/Components/BaseComponent.h
        include/library/Components/CameraComponent.h
        include/library/Components/DrawableComponent.h
        include/library/Components/FirstPersonCameraComponent.h
        include/library/Components/FpsComponent.h
        include/library/Components/GridComponent.h
        include/library/Components/KeyboardComponent.h
        include/library/Components/MouseComponent.h
        include/library/Components/SceneComponent.h
        include/library/Components/SkyboxComponent.h
        include/library/Components/TextComponent.h
        include/library/Components/UIComponent.h

        include/library/DirectXForwardDeclarations.h

        # effect
        include/library/effect/Effect.h
        include/library/effect/EffectMaterial.h
        include/library/effect/EffectPass.h
        include/library/effect/EffectTechnique.h
        include/library/effect/EffectVariable.h

        # materials
        include/library/materials/SkyboxMaterial.h

        include/library/Exception.h
        include/library/Key.h
        include/library/Logger.h
        include/library/Math.h
        include/library/Matrix.h
        include/library/Mesh.h
        include/library/Model.h
        include/library/ModelMaterial.h
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
        src/Color.cpp

        # components
        src/Components/BaseComponent.cpp
        src/Components/CameraComponent.cpp
        src/Components/DrawableComponent.cpp
        src/Components/FirstPersonCameraComponent.cpp
        src/Components/FpsComponent.cpp
        src/Components/GridComponent.cpp
        src/Components/KeyboardComponent.cpp
        src/Components/MouseComponent.cpp
        src/Components/SceneComponent.cpp
        src/Components/SkyboxComponent.cpp
        src/Components/TextComponent.cpp
        src/Components/UIComponent.cpp

        # effect
        src/effect/Effect.cpp
        src/effect/EffectMaterial.cpp
        src/effect/EffectPass.cpp
        src/effect/EffectTechnique.cpp
        src/effect/EffectVariable.cpp

        # materials
        src/materials/SkyboxMaterial.cpp

        src/Exception.cpp
        src/Logger.cpp
        src/Matrix.cpp
        src/ModelMaterial.cpp
        src/Mesh.cpp
        src/Model.cpp
        src/Path.cpp
        src/RenderStatesStorage.cpp
        src/ServiceContainer.cpp
        src/Stopwatch.cpp
        src/Utils.cpp
        src/Vector.cpp

        # precompiled header
        src/StdAfx.h
        src/StdAfx.cpp
    )

    add_library(library STATIC ${LIBRARY_SOURCES})

    target_include_directories(library
        PUBLIC
            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
            src
    )

    target_link_libraries(library effects11 directxtk assimp d3d11 d3dcompiler dxguid dinput8)

    target_compile_options(library
        PRIVATE
            /GR- # disable rtti
            /YuStdAfx.h
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

    set_source_files_properties(src/StdAfx.cpp PROPERTIES COMPILE_FLAGS /YcStdAfx.h)

    # ide filters
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBRARY_SOURCES})
endif()