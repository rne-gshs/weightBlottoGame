cmake_minimum_required(VERSION 3.24)
project(blottoGame)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-O3)
# add_compile_options(-march=alderlake)
add_compile_options(-march=icelake-server)

add_executable(blottoGame main.cpp blotto.cpp blotto.h database.cpp database.h)#
