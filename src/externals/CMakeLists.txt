cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})

# copy cmakelists to libs and add them to solution
file(GLOB cmakelists RELATIVE ${cur_dir} ${cur_dir}/*.CMakeLists.txt)
foreach(cmakelist ${cmakelists})
    get_filename_component(lib ${cmakelist} NAME_WE)

    string(FIND ${cmakelist} ".CMakeLists.txt" cmakelist_path_part_size REVERSE)
    string(SUBSTRING ${cmakelist} 0 ${cmakelist_path_part_size} cmakelist_path_part)
    string(REPLACE "." "/" lib_cmakelist_relative_path ${cmakelist_path_part})

    set(cmakelist_path ${cur_dir}/${cmakelist})
    set(lib_cmakelist_path ${cur_dir}/${lib_cmakelist_relative_path}/CMakeLists.txt)

    if (IS_DIRECTORY ${cur_dir}/${lib})
        set(override FALSE)

        if(NOT EXISTS ${lib_cmakelist_path})
            set(override TRUE)
        else()
            FILE(MD5 ${cmakelist_path} hash_new)
            FILE(MD5 ${lib_cmakelist_path} hash_old)

            if (NOT ${hash_new} STREQUAL ${hash_old})
                set(override TRUE)
            endif()
        endif()

        if(${override})
            FILE(READ ${cmakelist_path} file)
            FILE(WRITE ${lib_cmakelist_path} ${file})
        endif()
    endif()
endforeach()

# currently added manually
add_subdirectory(directxtk)
add_subdirectory(directxtex)
add_subdirectory(effects11)

if(APPLICATION)
    set (BUILD_SHARED_LIBS FALSE)
    set (ASSIMP_BUILD_ASSIMP_TOOLS FALSE)
    set (ASSIMP_BUILD_SAMPLES FALSE)
    set (ASSIMP_BUILD_TESTS FALSE)
    set (ASSIMP_BUILD_ZLIB TRUE)
    add_subdirectory(assimp)
endif()