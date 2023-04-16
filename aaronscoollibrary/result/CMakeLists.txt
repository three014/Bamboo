cmake_minimum_required(VERSION 3.1)
project(result)

set(
    result_SOURCES 
    src/result.c
)

add_library(
    ${PROJECT_NAME} 
    ${result_SOURCES}
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
