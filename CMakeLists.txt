cmake_minimum_required(VERSION 3.15)
project(wims LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-O2 -Wall -Wextra)
endif()

add_executable(wims wims.cpp console_func.cpp)
