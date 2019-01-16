cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

if(APPLICATION)
    project(demo VERSION 0.1)

    set(DEMO_SOURCES
        # src
            src/Program.cpp
            src/Application.h
            src/Application.cpp

            # components
                # without materials
                src/components/CubeComponent.h
                src/components/CubeComponent.cpp

                src/components/TriangleComponent.h
                src/components/TriangleComponent.cpp

                src/components/ModelComponent.h
                src/components/ModelComponent.cpp

                src/components/TextureModelComponent.h
                src/components/TextureModelComponent.cpp

                # with materials
                src/components/BasicEffectMaterialComponent.h
                src/components/BasicEffectMaterialComponent.cpp

                src/components/DiffuseLightingEffectMaterialComponent.h
                src/components/DiffuseLightingEffectMaterialComponent.cpp

                src/components/DisplacementMappingEffectMaterialComponent.h
                src/components/DisplacementMappingEffectMaterialComponent.cpp

                src/components/EnvironmentMappingEffectMaterialComponent.h
                src/components/EnvironmentMappingEffectMaterialComponent.cpp

                src/components/FogEffectMaterialComponent.h
                src/components/FogEffectMaterialComponent.cpp

                src/components/NormalMappingEffectMaterialComponent.h
                src/components/NormalMappingEffectMaterialComponent.cpp

                src/components/PointLightEffectMaterialComponent.h
                src/components/PointLightEffectMaterialComponent.cpp

                src/components/SpotlightEffectMaterialComponent.h
                src/components/SpotlightEffectMaterialComponent.cpp

                src/components/TextureMappingEffectMaterialComponent.h
                src/components/TextureMappingEffectMaterialComponent.cpp

                src/components/TransparencyMappingEffectMaterialComponent.h
                src/components/TransparencyMappingEffectMaterialComponent.cpp
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