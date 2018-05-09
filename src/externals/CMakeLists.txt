cmake_minimum_required(VERSION 3.2 FATAL_ERROR)

set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})

# copy cmakelists to libs and add them to solution
file(GLOB children RELATIVE ${cur_dir} ${cur_dir}/*)
foreach(child ${children})
    if (IS_DIRECTORY ${cur_dir}/${child})
        set(lib ${child})

        if(NOT EXISTS ${lib}/CMakeLists.txt)
            FILE(READ ${lib}.cmakelists file)
            FILE(WRITE ${lib}/CMakeLists.txt ${file})
        endif()

        add_subdirectory(${lib})
    endif()
endforeach()
