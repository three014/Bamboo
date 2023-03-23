cmake_minimum_required(VERSION 3.1)
project(option)

set(
    option_SOURCES 
    src/option.c
    src/option_i32.c
    src/option_u32.c
)

add_library(
    ${PROJECT_NAME} 
    ${option_SOURCES}
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
