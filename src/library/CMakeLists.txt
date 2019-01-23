cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_SOURCES
        # include
            # components
                include/library/components/Component.h

                include/library/components/CameraComponent.h
                include/library/components/ConcreteMaterialComponent.hpp
                include/library/components/DirectionalLightComponent.h
                include/library/components/DrawableComponent.h
                include/library/components/FirstPersonCameraComponent.h
                include/library/components/FpsComponent.h
                include/library/components/FullScreenQuadComponent.h
                include/library/components/GridComponent.h
                include/library/components/InputComponent.h
                include/library/components/InputReceivableComponent.h
                include/library/components/KeyboardComponent.h
                include/library/components/LightComponent.h
                include/library/components/IMaterialComponent.h
                include/library/components/MaterialComponent.h
                include/library/components/MaterialComponentGlue.hpp
                include/library/components/MouseComponent.h
                include/library/components/PointLightComponent.h
                include/library/components/PostProcessingComponent.h
                include/library/components/PostProcessingComponentGlue.hpp
                include/library/components/ProxyModelComponent.h
                include/library/components/SceneComponent.h
                include/library/components/SkyboxComponent.h
                include/library/components/SpotLightComponent.h
                include/library/components/TextComponent.h

            # effect
                include/library/effect/Effect.h

                include/library/effect/EffectPass.h
                include/library/effect/EffectTechnique.h
                include/library/effect/EffectVariable.h

            # materials
                include/library/materials/Material.h

                include/library/materials/BasicMaterial.h
                include/library/materials/ColorFilterMaterial.h
                include/library/materials/DiffuseLightingMaterial.h
                include/library/materials/DisplacementMappingMaterial.h
                include/library/materials/EnvironmentMappingMaterial.h
                include/library/materials/FogMaterial.h
                include/library/materials/GaussianBlurMaterial.h
                include/library/materials/NormalMappingMaterial.h
                include/library/materials/PointLightMaterial.h
                include/library/materials/PostProcessingMaterial.h
                include/library/materials/SkyboxMaterial.h
                include/library/materials/SpotlightMaterial.h
                include/library/materials/TextureMappingMaterial.h
                include/library/materials/TransparencyMappingMaterial.h

            # math
                include/library/math/Math.h

                include/library/math/Matrix.h
                include/library/math/Vector.h

            include/library/Application.h
            include/library/BlendStateHolder.h
            include/library/Color.h
            include/library/CommonTypes.h
            include/library/DirectXForwardDeclarations.h
            include/library/Exception.h
            include/library/FullScreenRenderTarget.h
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
                src/components/Component.cpp

                src/components/CameraComponent.cpp
                src/components/DirectionalLightComponent.cpp
                src/components/DrawableComponent.cpp
                src/components/FirstPersonCameraComponent.cpp
                src/components/FpsComponent.cpp
                src/components/FullScreenQuadComponent.cpp
                src/components/GridComponent.cpp
                src/components/InputComponent.cpp
                src/components/InputReceivableComponent.cpp
                src/components/KeyboardComponent.cpp
                src/components/LightComponent.cpp
                src/components/MaterialComponent.cpp
                src/components/MouseComponent.cpp
                src/components/PointLightComponent.cpp
                src/components/PostProcessingComponent.cpp
                src/components/ProxyModelComponent.cpp
                src/components/SceneComponent.cpp
                src/components/SkyboxComponent.cpp
                src/components/SpotLightComponent.cpp
                src/components/TextComponent.cpp


            # effect
                src/effect/Effect.cpp

                src/effect/EffectPass.cpp
                src/effect/EffectTechnique.cpp
                src/effect/EffectVariable.cpp

            # materials
                src/materials/Material.cpp

                src/materials/BasicMaterial.cpp
                src/materials/ColorFilterMaterial.cpp
                src/materials/DiffuseLightingMaterial.cpp
                src/materials/DisplacementMappingMaterial.cpp
                src/materials/EnvironmentMappingMaterial.cpp
                src/materials/FogMaterial.cpp
                src/materials/GaussianBlurMaterial.cpp
                src/materials/NormalMappingMaterial.cpp
                src/materials/PointLightMaterial.cpp
                src/materials/PostProcessingMaterial.cpp
                src/materials/SkyboxMaterial.cpp
                src/materials/SpotlightMaterial.cpp
                src/materials/TextureMappingMaterial.cpp
                src/materials/TransparencyMappingMaterial.cpp

            # math
                src/math/Matrix.cpp
                src/math/Vector.cpp

            src/Application.cpp
            src/BlendStateHolder.cpp
            src/Color.cpp
            src/Exception.cpp
            src/FullScreenRenderTarget.cpp
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
            /wd4250 # disable dominance via inheritance warning (VS specific)
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