cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

project(fresh
    VERSION 0.1
    LANGUAGES C)

set(SOURCES
    ./src/main.c)

add_executable(fresh "${SOURCES}")

target_include_directories(fresh PUBLIC ./include)
configure_file(fresh_config.h.in "${PROJECT_SOURCE_DIR}/include/fresh_config.h")