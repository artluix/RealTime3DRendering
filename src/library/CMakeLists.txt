cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_SOURCES
        # include
            # components
                include/library/components/CameraComponent.h
                include/library/components/Component.h
                include/library/components/DirectionalLightComponent.h
                include/library/components/DrawableComponent.h
                include/library/components/FirstPersonCameraComponent.h
                include/library/components/FpsComponent.h
                include/library/components/GridComponent.h
                include/library/components/InputComponent.h
                include/library/components/InputReceivableComponent.h
                include/library/components/KeyboardComponent.h
                include/library/components/LightComponent.h
                include/library/components/MouseComponent.h
                include/library/components/PointLightComponent.h
                include/library/components/ProxyModelComponent.h
                include/library/components/SceneComponent.h
                include/library/components/SkyboxComponent.h
                include/library/components/SpotLightComponent.h
                include/library/components/TextComponent.h
                include/library/components/UIComponent.h

            # effect
                # materials
                    include/library/effect/materials/BasicEffectMaterial.h
                    include/library/effect/materials/DiffuseLightingEffectMaterial.h
                    include/library/effect/materials/DisplacementMappingEffectMaterial.h
                    include/library/effect/materials/EnvironmentMappingEffectMaterial.h
                    include/library/effect/materials/NormalMappingEffectMaterial.h
                    include/library/effect/materials/PointLightEffectMaterial.h
                    include/library/effect/materials/SkyboxEffectMaterial.h
                    include/library/effect/materials/SpotlightEffectMaterial.h
                    include/library/effect/materials/TextureMappingEffectMaterial.h
                    include/library/effect/materials/TransparencyMappingEffectMaterial.h

                include/library/effect/Effect.h
                include/library/effect/EffectMaterial.h
                include/library/effect/EffectPass.h
                include/library/effect/EffectTechnique.h
                include/library/effect/EffectVariable.h

            # math
                include/library/math/Math.h
                include/library/math/Matrix.h
                include/library/math/Vector.h

            include/library/Application.h
            include/library/Color.h
            include/library/CommonTypes.h
            include/library/DirectXForwardDeclarations.h
            include/library/Exception.h
            include/library/Key.h
            include/library/Logger.h
            include/library/Mesh.h
            include/library/Model.h
            include/library/ModelMaterial.h
            include/library/NonCopyable.hpp
            include/library/NonConstructible.hpp
            include/library/Path.h
            include/library/Renderer.h
            include/library/RasterizerStateHolder.h
            include/library/RTTI.hpp
            include/library/ServiceContainer.h
            include/library/Stopwatch.h
            include/library/Time.h
            include/library/Utils.h
            include/library/VertexTypes.h

        # src
            # components
                src/components/CameraComponent.cpp
                src/components/Component.cpp
                src/components/DirectionalLightComponent.cpp
                src/components/DrawableComponent.cpp
                src/components/FirstPersonCameraComponent.cpp
                src/components/FpsComponent.cpp
                src/components/GridComponent.cpp
                src/components/InputComponent.cpp
                src/components/InputReceivableComponent.cpp
                src/components/KeyboardComponent.cpp
                src/components/LightComponent.cpp
                src/components/MouseComponent.cpp
                src/components/PointLightComponent.cpp
                src/components/ProxyModelComponent.cpp
                src/components/SceneComponent.cpp
                src/components/SkyboxComponent.cpp
                src/components/SpotLightComponent.cpp
                src/components/TextComponent.cpp
                src/components/UIComponent.cpp


            # effect
                # materials
                    src/effect/materials/BasicEffectMaterial.cpp
                    src/effect/materials/DiffuseLightingEffectMaterial.cpp
                    src/effect/materials/DisplacementMappingEffectMaterial.cpp
                    src/effect/materials/EnvironmentMappingEffectMaterial.cpp
                    src/effect/materials/NormalMappingEffectMaterial.cpp
                    src/effect/materials/PointLightEffectMaterial.cpp
                    src/effect/materials/SkyboxEffectMaterial.cpp
                    src/effect/materials/SpotlightEffectMaterial.cpp
                    src/effect/materials/TextureMappingEffectMaterial.cpp
                    src/effect/materials/TransparencyMappingEffectMaterial.cpp

                src/effect/Effect.cpp
                src/effect/EffectMaterial.cpp
                src/effect/EffectPass.cpp
                src/effect/EffectTechnique.cpp
                src/effect/EffectVariable.cpp

            # math
            src/math/Matrix.cpp
            src/math/Vector.cpp

            src/Application.cpp
            src/Color.cpp
            src/Exception.cpp
            src/Logger.cpp
            src/Mesh.cpp
            src/Model.cpp
            src/ModelMaterial.cpp
            src/Path.cpp
            src/RasterizerStateHolder.cpp
            src/Renderer.cpp
            src/ServiceContainer.cpp
            src/Stopwatch.cpp
            src/Utils.cpp

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