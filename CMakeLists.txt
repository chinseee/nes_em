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
    add_compile_options(-O3 -Wall -Wextra -Wpedantic -Werror -Wno-gnu-line-marker -fverbose-asm)
endif()

add_library(nes_em_core
    src/nes_em/cartridge/cartridge.cpp
    src/nes_em/cartridge/nrom_cart.cpp
    src/nes_em/nes_file.cpp
    src/nes_em/bus.cpp
    src/nes_em/controller.cpp
    src/nes_em/cpu.cpp 
    src/nes_em/cpu_debug.cpp 
    src/nes_em/nes.cpp
    src/nes_em/ppu.cpp 
)

target_include_directories(nes_em_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_options(nes_em_core PRIVATE -save-temps=obj)

find_package(SDL3 REQUIRED CONFIG)

add_library(nes_client src/nes_client/client.cpp)
target_include_directories(nes_client PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(nes_client PUBLIC SDL3::SDL3)


add_executable(nes_em src/main.cpp)
target_link_libraries(nes_em PRIVATE nes_client nes_em_core)
target_compile_options(nes_em PRIVATE -save-temps=obj)