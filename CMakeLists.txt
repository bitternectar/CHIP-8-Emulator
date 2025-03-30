cmake_minimum_required(VERSION 3.30)
project(CHIP_8 C)

set(CMAKE_C_STANDARD 11)

# Найти библиотеку SDL2
find_package(SDL2 REQUIRED)

# Включить заголовочные файлы SDL2
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(CHIP_8 main.c
        instructions.h
        utils.h
        constants.h
        instructions.c
        chip8.h
        chip8.c
        sdl.h)
# Связать исполняемый файл с библиотекой SDL2
target_link_libraries(CHIP_8 ${SDL2_LIBRARIES})