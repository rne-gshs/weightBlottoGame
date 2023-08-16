cmake_minimum_required(VERSION 3.24)
project(blottoGame)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3)
# add_compile_options(-march=alderlake)
# add_compile_options(-march=icelake-server)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(OptimizeForArchitecture)

#[[find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()]]

add_executable(blottoGame main.cpp blotto.cpp blotto.h database.cpp database.h)
