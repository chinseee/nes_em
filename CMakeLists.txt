cmake_minimum_required(VERSION 3.28)
project(nes_em
    VERSION 0.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

if(MSVC)
    add_compile_options(/O2 /W4 /WX)
else()
    add_compile_options(-O3 -Wall -Wextra -Wpedantic -Werror -fverbose-asm)
endif()

add_executable(nes_em src/main.cpp src/cpu.cpp src/cpu_debug.cpp src/nes_file.cpp)
target_include_directories(nes_em PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(nes_em PRIVATE -save-temps=obj)