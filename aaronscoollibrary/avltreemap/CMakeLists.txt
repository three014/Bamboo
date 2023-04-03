cmake_minimum_required(VERSION 3.0)
project(avltreemap)

add_library(${PROJECT_NAME} src/avltreemap.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
)
