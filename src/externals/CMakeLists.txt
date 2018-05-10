cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})

# copy cmakelists to libs and add them to solution
file(GLOB children RELATIVE ${cur_dir} ${cur_dir}/*)
foreach(child ${children})
    if (IS_DIRECTORY ${cur_dir}/${child})
        set(lib ${child})

        set(override FALSE)

        if(NOT EXISTS ${cur_dir}/${lib}/CMakeLists.txt)
            set(override TRUE)
        else()
            FILE(MD5 ${cur_dir}/${lib}.cmakelists hash_new)
            FILE(MD5 ${cur_dir}/${lib}/CMakeLists.txt hash_old)

            if (NOT ${hash_new} STREQUAL ${hash_old})
                set(override TRUE)
            endif()
        endif()

        if(${override})
            FILE(READ ${lib}.cmakelists file)
            FILE(WRITE ${lib}/CMakeLists.txt ${file})
        endif()

        add_subdirectory(${lib})
    endif()
endforeach()
