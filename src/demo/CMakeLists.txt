cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(demo VERSION 0.1)

    set(DEMO_SOURCES
        # src
            src/DemoApplication.h
            src/DemoApplication.cpp
            src/Program.cpp

            src/DemoUtils.h
            src/DemoUtils.cpp

            src/LightsData.h

        # components

        # without materials
            src/Components/CubeDemo.h
            src/Components/CubeDemo.cpp

            src/Components/TriangleDemo.h
            src/Components/TriangleDemo.cpp

            src/Components/ModelDemo.h
            src/Components/ModelDemo.cpp

            src/Components/TextureModelDemo.h
            src/Components/TextureModelDemo.cpp

        # with materials
            src/Components/AnimationDemo.h
            src/Components/AnimationDemo.cpp

            src/Components/BasicMaterialDemo.h
            src/Components/BasicMaterialDemo.cpp

            src/Components/BasicTessellationDemo.h
            src/Components/BasicTessellationDemo.cpp

            src/Components/BloomDemo.h
            src/Components/BloomDemo.cpp

            src/Components/ColorFilterDemo.h
            src/Components/ColorFilterDemo.cpp

            src/Components/ComputeShaderDemo.h
            src/Components/ComputeShaderDemo.cpp

            src/Components/DiffuseLightingDemo.h
            src/Components/DiffuseLightingDemo.cpp

            src/Components/DisplacementMappingDemo.h
            src/Components/DisplacementMappingDemo.cpp

            src/Components/DistortionMappingDemo.h
            src/Components/DistortionMappingDemo.cpp

            src/Components/EnvironmentMappingDemo.h
            src/Components/EnvironmentMappingDemo.cpp

            src/Components/FogDemo.h
            src/Components/FogDemo.cpp

            src/Components/GaussianBlurDemo.h
            src/Components/GaussianBlurDemo.cpp

            src/Components/GeometryShaderDemo.h
            src/Components/GeometryShaderDemo.cpp

            src/Components/InstancingDemo.h
            src/Components/InstancingDemo.cpp

            src/Components/ModelTessellationDemo.h
            src/Components/ModelTessellationDemo.cpp

            src/Components/MultipleLightsDemo.h
            src/Components/MultipleLightsDemo.cpp

            src/Components/NormalMappingDemo.h
            src/Components/NormalMappingDemo.cpp

            src/Components/PointLightDemo.h
            src/Components/PointLightDemo.cpp

            src/Components/ProjectiveTextureMappingDemo.h
            src/Components/ProjectiveTextureMappingDemo.cpp

            src/Components/ProjectiveTextureMappingDepthMapDemo.h
            src/Components/ProjectiveTextureMappingDepthMapDemo.cpp

            src/Components/QuadHeightMapTessellationDemo.h
            src/Components/QuadHeightMapTessellationDemo.cpp

            src/Components/ShadowMappingDemo.h
            src/Components/ShadowMappingDemo.cpp

            src/Components/SpotlightDemo.h
            src/Components/SpotlightDemo.cpp

            src/Components/TextureMappingDemo.h
            src/Components/TextureMappingDemo.cpp

            src/Components/TransparencyMappingDemo.h
            src/Components/TransparencyMappingDemo.cpp

        # materials
            src/Materials/ColorFilterMaterial.h
            src/Materials/ColorFilterMaterial.cpp

            src/Materials/ComputeShaderMaterial.h
            src/Materials/ComputeShaderMaterial.cpp

            src/Materials/DepthMapMaterial.h
            src/Materials/DepthMapMaterial.cpp

            src/Materials/DiffuseLightingMaterial.h
            src/Materials/DiffuseLightingMaterial.cpp

            src/Materials/DisplacementMappingMaterial.h
            src/Materials/DisplacementMappingMaterial.cpp

            src/Materials/DistortionMappingMaterial.h
            src/Materials/DistortionMappingMaterial.cpp

            src/Materials/EnvironmentMappingMaterial.h
            src/Materials/EnvironmentMappingMaterial.cpp

            src/Materials/FogMaterial.h
            src/Materials/FogMaterial.cpp

            src/Materials/InstancingPointLightMaterial.h
            src/Materials/InstancingPointLightMaterial.cpp

            src/Materials/LightMaterial.h
            src/Materials/LightMaterial.cpp

            src/Materials/ModelTessellationMaterial.h
            src/Materials/ModelTessellationMaterial.cpp

            src/Materials/MultipleLightsMaterial.h
            src/Materials/MultipleLightsMaterial.cpp

            src/Materials/NormalMappingMaterial.h
            src/Materials/NormalMappingMaterial.cpp

            src/Materials/PointSpriteMaterial.h
            src/Materials/PointSpriteMaterial.cpp

            src/Materials/ProjectiveTextureMappingMaterial.h
            src/Materials/ProjectiveTextureMappingMaterial.cpp

            src/Materials/QuadHeightMapTessellationMaterial.h
            src/Materials/QuadHeightMapTessellationMaterial.cpp

            src/Materials/ShadowMappingMaterial.h
            src/Materials/ShadowMappingMaterial.cpp

            src/Materials/SkinnedModelMaterial.h
            src/Materials/SkinnedModelMaterial.cpp

            src/Materials/TessellationMaterial.h
            src/Materials/TextureMappingMaterial.cpp

            src/Materials/TextureMappingMaterial.h
            src/Materials/TessellationMaterial.cpp

            src/Materials/TransparencyMappingMaterial.h
            src/Materials/TransparencyMappingMaterial.cpp
    )

    add_executable(demo WIN32 ${DEMO_SOURCES})

    target_include_directories(demo
        PRIVATE
            src
    )

    target_link_libraries(demo library)

    target_compile_options(demo
        PUBLIC
            /GR- # disable rtti
            /MP
            /std:c++17
            /wd4250 # disable dominance via inheritance warning (VS specific)
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
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DEMO_SOURCES})

    # change target path instead
    set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_DIR}/${CMAKE_VS_PLATFORM_NAME})
    set_target_properties(demo PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_DIR}/${CMAKE_VS_PLATFORM_NAME})

endif()