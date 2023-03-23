cmake_minimum_required(VERSION 3.0)
project(avltreeset)

add_library(${PROJECT_NAME} src/avltreeset.c)
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC option interface)

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
