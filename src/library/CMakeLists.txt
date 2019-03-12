cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_SOURCES
        # include
            # Components
                include/library/Components/Component.h

                include/library/Components/BloomComponent.h
                include/library/Components/CameraComponent.h
                include/library/Components/ConcreteMaterialPostProcessingComponent.hpp
                include/library/Components/ConcreteMaterialSceneComponent.hpp
                include/library/Components/DirectionalLightComponent.h
                include/library/Components/DrawableComponent.h
                include/library/Components/FirstPersonCameraComponent.h
                include/library/Components/FpsComponent.h
                include/library/Components/FullScreenQuadComponent.h
                include/library/Components/GaussianBlurComponent.h
                include/library/Components/GridComponent.h
                include/library/Components/InputComponent.h
                include/library/Components/InputReceivableComponent.h
                include/library/Components/KeyboardComponent.h
                include/library/Components/LightComponent.h
                include/library/Components/MouseComponent.h
                include/library/Components/OrthographicProjectorComponent.h
                include/library/Components/PointLightComponent.h
                include/library/Components/PostProcessingComponent.h
                include/library/Components/PerspectiveProjectorComponent.h
                include/library/Components/ProjectorComponent.h
                include/library/Components/ProxyModelComponent.h
                include/library/Components/RenderableFrustumComponent.h
                include/library/Components/SceneComponent.h
                include/library/Components/SkyboxComponent.h
                include/library/Components/SpotLightComponent.h
                include/library/Components/TextComponent.h
                include/library/Components/UIComponent.h

            # Effect
                include/library/Effect/Effect.h

                include/library/Effect/EffectPass.h
                include/library/Effect/EffectTechnique.h
                include/library/Effect/EffectVariable.h

            # Materials
                include/library/Materials/Material.h

                include/library/Materials/BasicMaterial.h
                include/library/Materials/BloomMaterial.h
                include/library/Materials/ColorFilterMaterial.h
                include/library/Materials/DepthMapMaterial.h
                include/library/Materials/DiffuseLightingMaterial.h
                include/library/Materials/DirectionalLightMaterial.h
                include/library/Materials/DirectionalShadowMappingMaterial.h
                include/library/Materials/DisplacementMappingMaterial.h
                include/library/Materials/DistortionMappingMaterial.h
                include/library/Materials/EnvironmentMappingMaterial.h
                include/library/Materials/FogMaterial.h
                include/library/Materials/GaussianBlurMaterial.h
                include/library/Materials/LightMaterial.h
                include/library/Materials/NormalMappingMaterial.h
                include/library/Materials/PointLightMaterial.h
                include/library/Materials/PostProcessingMaterial.h
                include/library/Materials/ProjectiveTextureMappingMaterial.h
                include/library/Materials/ShadowMappingMaterial.h
                include/library/Materials/SkyboxMaterial.h
                include/library/Materials/SpotlightMaterial.h
                include/library/Materials/TextureMappingMaterial.h
                include/library/Materials/TransparencyMappingMaterial.h

            # Math
                include/library/Math/Math.h

                include/library/Math/Matrix.h
                include/library/Math/Vector.h

            # RenderTargets
                include/library/RenderTargets/RenderTarget.h

                include/library/RenderTargets/DepthMapRenderTarget.h
                include/library/RenderTargets/FullScreenRenderTarget.h

            include/library/Application.h
            include/library/BlendStates.h
            include/library/Color.h
            include/library/CommonTypes.h
            include/library/DirectXForwardDeclarations.h
            include/library/Exception.h
            include/library/Frustum.h
            include/library/Key.h
            include/library/Logger.h
            include/library/Mesh.h
            include/library/Model.h
            include/library/ModelMaterial.h
            include/library/NonCopyable.hpp
            include/library/NonConstructible.hpp
            include/library/Path.h
            include/library/Ray.h
            include/library/Renderer.h
            include/library/RasterizerStates.h
            include/library/RTTI.hpp
            include/library/SamplerStates.h
            include/library/ServiceContainer.h
            include/library/Stopwatch.h
            include/library/Time.h
            include/library/Utils.h
            include/library/VertexTypes.h

        # src
            # Components
                src/Components/Component.cpp

                src/Components/BloomComponent.cpp
                src/Components/CameraComponent.cpp
                src/Components/DirectionalLightComponent.cpp
                src/Components/DrawableComponent.cpp
                src/Components/FirstPersonCameraComponent.cpp
                src/Components/FpsComponent.cpp
                src/Components/FullScreenQuadComponent.cpp
                src/Components/GaussianBlurComponent.cpp
                src/Components/GridComponent.cpp
                src/Components/InputComponent.cpp
                src/Components/InputReceivableComponent.cpp
                src/Components/KeyboardComponent.cpp
                src/Components/LightComponent.cpp
                src/Components/MouseComponent.cpp
                src/Components/OrthographicProjectorComponent.cpp
                src/Components/PointLightComponent.cpp
                src/Components/PostProcessingComponent.cpp
                src/Components/PerspectiveProjectorComponent.cpp
                src/Components/ProjectorComponent.cpp
                src/Components/ProxyModelComponent.cpp
                src/Components/RenderableFrustumComponent.cpp
                src/Components/SceneComponent.cpp
                src/Components/SkyboxComponent.cpp
                src/Components/SpotLightComponent.cpp
                src/Components/TextComponent.cpp
                src/Components/UIComponent.cpp

            # Effect
                src/Effect/Effect.cpp

                src/Effect/EffectPass.cpp
                src/Effect/EffectTechnique.cpp
                src/Effect/EffectVariable.cpp

            # Materials
                src/Materials/Material.cpp

                src/Materials/BasicMaterial.cpp
                src/Materials/BloomMaterial.cpp
                src/Materials/ColorFilterMaterial.cpp
                src/Materials/DepthMapMaterial.cpp
                src/Materials/DiffuseLightingMaterial.cpp
                src/Materials/DirectionalLightMaterial.cpp
                src/Materials/DirectionalShadowMappingMaterial.cpp
                src/Materials/DisplacementMappingMaterial.cpp
                src/Materials/DistortionMappingMaterial.cpp
                src/Materials/EnvironmentMappingMaterial.cpp
                src/Materials/FogMaterial.cpp
                src/Materials/GaussianBlurMaterial.cpp
                src/Materials/LightMaterial.cpp
                src/Materials/NormalMappingMaterial.cpp
                src/Materials/PointLightMaterial.cpp
                src/Materials/PostProcessingMaterial.cpp
                src/Materials/ProjectiveTextureMappingMaterial.cpp
                src/Materials/ShadowMappingMaterial.cpp
                src/Materials/SkyboxMaterial.cpp
                src/Materials/SpotlightMaterial.cpp
                src/Materials/TextureMappingMaterial.cpp
                src/Materials/TransparencyMappingMaterial.cpp

            # Math
                src/Math/Matrix.cpp
                src/Math/Vector.cpp

            # RenderTargets
                src/RenderTargets/RenderTarget.cpp

                src/RenderTargets/DepthMapRenderTarget.cpp
                src/RenderTargets/FullScreenRenderTarget.cpp

            src/Application.cpp
            src/BlendStates.cpp
            src/Color.cpp
            src/Exception.cpp
            src/Frustum.cpp
            src/Logger.cpp
            src/Mesh.cpp
            src/Model.cpp
            src/ModelMaterial.cpp
            src/Path.cpp
            src/RasterizerStates.cpp
            src/Renderer.cpp
            src/SamplerStates.cpp
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