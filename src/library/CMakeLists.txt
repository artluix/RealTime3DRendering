cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(library VERSION 0.1)

    set(LIBRARY_SOURCES
        # include
            # Components
                include/library/Components/Component.h

                include/library/Components/AnimationPlayerComponent.h
                include/library/Components/BloomComponent.h
                include/library/Components/CameraComponent.h
                include/library/Components/CubeComponent.h
                include/library/Components/ConcreteMaterialPostProcessingComponent.h
                include/library/Components/ConcreteMaterialPrimitiveComponent.h
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
                include/library/Components/PointLightComponent.h
                include/library/Components/PostProcessingComponent.h
                include/library/Components/PrimitiveComponent.h
                include/library/Components/ProjectorComponent.h
                include/library/Components/ProxyModelComponent.h
                include/library/Components/RenderableFrustumComponent.h
                include/library/Components/SceneComponent.h
                include/library/Components/SimplePrimitiveComponent.h
                include/library/Components/SkyboxComponent.h
                include/library/Components/SpotlightComponent.h
                include/library/Components/TextComponent.h
                include/library/Components/ToneMappingComponent.h
                include/library/Components/UIComponent.h

            # Effect
                include/library/Effect/Effect.h

                include/library/Effect/EffectPass.h
                include/library/Effect/EffectTechnique.h
                include/library/Effect/EffectVariable.h
                include/library/Effect/EffectVariable-inl.h

            # Materials
                include/library/Materials/Material.h
                include/library/Materials/MaterialUpdateFunction.h

                include/library/Materials/BasicMaterial.h
                include/library/Materials/BloomMaterial.h
                include/library/Materials/EquirectangularToCubeMapMaterial.h
                include/library/Materials/GaussianBlurMaterial.h
                include/library/Materials/IrradianceConvolutionMaterial.h
                include/library/Materials/PostProcessingMaterial.h
                include/library/Materials/SkyboxMaterial.h
                include/library/Materials/ToneMappingMaterial.h

            # Math
                include/library/Math/Math.h
                include/library/Math/MathFwd.h

                include/library/Math/Color.h
                include/library/Math/Direction.h
                include/library/Math/DxMath.h
                include/library/Math/DXMathFwd.h
                include/library/Math/Frustum.h
                include/library/Math/Interval.h
                include/library/Math/MathConstants.h
                include/library/Math/MathUtils.h
                include/library/Math/Matrix.h
                include/library/Math/Matrix-inl.h
                include/library/Math/Plane.h
                include/library/Math/Quaternion.h
                include/library/Math/Ray.h
                include/library/Math/Transform.h
                include/library/Math/Vector.h
                include/library/Math/Vector-inl.h

            # Model
                include/library/Model/Model.h

                include/library/Model/AnimationClip.h
                include/library/Model/AnimationDef.h
                include/library/Model/Bone.h
                include/library/Model/BoneAnimation.h
                include/library/Model/Keyframe.h
                include/library/Model/Mesh.h
                include/library/Model/ModelMaterial.h
                include/library/Model/SceneNode.h
                include/library/Model/TextureType.h

            # Render
                include/library/Render/BlendStates.h
                include/library/Render/BufferData.h
                include/library/Render/Misc.h
                include/library/Render/PrimitiveData.h
                include/library/Render/RenderBuffers.h
                include/library/Render/Renderer.h
                include/library/Render/RasterizerStates.h
                include/library/Render/SamplerStates.h
                include/library/Render/Vertex.h
                include/library/Render/VertexElement.h
                include/library/Render/Viewport.h

            # RenderTargets
                include/library/RenderTargets/RenderTarget.h

                include/library/RenderTargets/DeferredLightingRenderTarget.h
                include/library/RenderTargets/DepthMapRenderTarget.h
                include/library/RenderTargets/FullScreenRenderTarget.h
                include/library/RenderTargets/MultipleRenderTarget.h
                include/library/RenderTargets/SingleRenderTarget.h

            include/library/Application.h
            include/library/Array.h
            include/library/ArrayBuffer.h
            include/library/Common.h
            include/library/ComPtr.h
            include/library/DxForward.h
            include/library/DynArray.h
            include/library/FxForward.h
            include/library/Key.h
            include/library/Logger.h
            include/library/NonCopyable.h
            include/library/NonConstructible.h
            include/library/Pair.h
            include/library/Path.h
            include/library/RTTI.h
            include/library/ServiceContainer.h
            include/library/Stopwatch.h
            include/library/Time.h
            include/library/TypeTraits.h
            include/library/Utils.h

        # src
            # Components
                src/Components/AnimationPlayerComponent.cpp
                src/Components/BloomComponent.cpp
                src/Components/CameraComponent.cpp
                src/Components/CubeComponent.cpp
                src/Components/DirectionalLightComponent.cpp
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
                src/Components/PointLightComponent.cpp
                src/Components/PostProcessingComponent.cpp
                src/Components/PrimitiveComponent.cpp
                src/Components/ProjectorComponent.cpp
                src/Components/ProxyModelComponent.cpp
                src/Components/RenderableFrustumComponent.cpp
                src/Components/SceneComponent.cpp
                src/Components/SimplePrimitiveComponent.cpp
                src/Components/SkyboxComponent.cpp
                src/Components/SpotlightComponent.cpp
                src/Components/TextComponent.cpp
                src/Components/ToneMappingComponent.cpp
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
                src/Materials/EquirectangularToCubeMapMaterial.cpp
                src/Materials/GaussianBlurMaterial.cpp
                src/Materials/IrradianceConvolutionMaterial.cpp
                src/Materials/PostProcessingMaterial.cpp
                src/Materials/SkyboxMaterial.cpp
                src/Materials/ToneMappingMaterial.cpp

            # Math
                src/Math/Color.cpp
                src/Math/Frustum.cpp
                src/Math/Matrix.cpp
                src/Math/Plane.cpp
                src/Math/Quaternion.cpp
                src/Math/Vector.cpp

            # Model
                src/Model/Model.cpp

                src/Model/AnimationClip.cpp
                src/Model/Bone.cpp
                src/Model/BoneAnimation.cpp
                src/Model/Keyframe.cpp
                src/Model/Mesh.cpp
                src/Model/ModelMaterial.cpp
                src/Model/SceneNode.cpp

            # Render
                src/Render/BlendStates.cpp
                src/Render/BufferData.cpp
                src/Render/RasterizerStates.cpp
                src/Render/RenderBuffers.cpp
                src/Render/Renderer.cpp
                src/Render/SamplerStates.cpp
                src/Render/Vertex.cpp

            # RenderTargets
                src/RenderTargets/RenderTarget.cpp

                src/RenderTargets/DeferredLightingRenderTarget.cpp
                src/RenderTargets/DepthMapRenderTarget.cpp
                src/RenderTargets/FullScreenRenderTarget.cpp
                src/RenderTargets/MultipleRenderTarget.cpp
                src/RenderTargets/SingleRenderTarget.cpp

            src/Application.cpp
            src/Logger.cpp
            src/Path.cpp
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

    target_link_libraries(library effects11 directxtk directxtex assimp d3d11 d3dcompiler dxguid dinput8)

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
            -DNOMINMAX

            # debug
            $<$<CONFIG:DEBUG>: -DDEBUG -D_DEBUG>
            # release
    )

    set_source_files_properties(src/StdAfx.cpp PROPERTIES COMPILE_FLAGS /YcStdAfx.h)

    # ide filters
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${LIBRARY_SOURCES})
endif()