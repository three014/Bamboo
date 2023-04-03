cmake_minimum_required(VERSION 3.1)
project(vector)

add_library(
    ${PROJECT_NAME} 
    src/vector.c 
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
