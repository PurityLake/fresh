cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

project(fresh
    VERSION 0.1
    LANGUAGES C)

set(SOURCES
    ./src/sexp.c
    ./src/main.c)

set(LIB_SOURCES
    ./src/sexp.c)

add_executable(freshi "${SOURCES}")
add_library(fresh "${LIB_SOURCES}")

target_include_directories(freshi PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(fresh PUBLIC "${PROJECT_SOURCE_DIR}/include")
configure_file(fresh_config.h.in "${PROJECT_SOURCE_DIR}/include/fresh_config.h")