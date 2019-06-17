cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(demo VERSION 0.1)

    set(DEMO_SOURCES
        # src
            src/DemoApplication.h
            src/DemoApplication.cpp
            src/Program.cpp

        # demo components

        # without materials
            src/CubeDemo.h
            src/CubeDemo.cpp

            src/TriangleDemo.h
            src/TriangleDemo.cpp

            src/ModelDemo.h
            src/ModelDemo.cpp

            src/TextureModelDemo.h
            src/TextureModelDemo.cpp

        # with materials
            src/AnimationDemo.h
            src/AnimationDemo.cpp

            src/BasicMaterialDemo.h
            src/BasicMaterialDemo.cpp

            # src/BasicTessellationDemo.h
            # src/BasicTessellationDemo.cpp

            src/BloomDemo.h
            src/BloomDemo.cpp

            src/ColorFilterDemo.h
            src/ColorFilterDemo.cpp

            src/DiffuseLightingDemo.h
            src/DiffuseLightingDemo.cpp

            src/DirectionalShadowMappingDemo.h
            src/DirectionalShadowMappingDemo.cpp

            src/DisplacementMappingDemo.h
            src/DisplacementMappingDemo.cpp

            src/DistortionMappingDemo.h
            src/DistortionMappingDemo.cpp

            src/EnvironmentMappingDemo.h
            src/EnvironmentMappingDemo.cpp

            src/FogDemo.h
            src/FogDemo.cpp

            src/GaussianBlurDemo.h
            src/GaussianBlurDemo.cpp

            # src/GeometryShaderDemo.h
            # src/GeometryShaderDemo.cpp

            src/NormalMappingDemo.h
            src/NormalMappingDemo.cpp

            src/PointLightDemo.h
            src/PointLightDemo.cpp

            src/ProjectiveTextureMappingDemo.h
            src/ProjectiveTextureMappingDemo.cpp

            src/ProjectiveTextureMappingDepthMapDemo.h
            src/ProjectiveTextureMappingDepthMapDemo.cpp

            src/ShadowMappingDemo.h
            src/ShadowMappingDemo.cpp

            src/SpotlightDemo.h
            src/SpotlightDemo.cpp

            src/TextureMappingDemo.h
            src/TextureMappingDemo.cpp

            src/TransparencyMappingDemo.h
            src/TransparencyMappingDemo.cpp
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