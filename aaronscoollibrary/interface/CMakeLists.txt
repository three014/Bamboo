cmake_minimum_required(VERSION 3.1)
project(interface)

add_library(
    ${PROJECT_NAME} 
    src/iterator/iterator.c
    src/iterator/constructor.c
    src/ordering/ordering.c
)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
