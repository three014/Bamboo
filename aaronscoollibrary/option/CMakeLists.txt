cmake_minimum_required(VERSION 3.1)
project(option)

set(
    option_SOURCES 
    src/option.c
    src/option_i8.c
    src/option_i16.c
    src/option_i32.c
    src/option_i64.c
    src/option_u8.c
    src/option_u16.c
    src/option_u32.c
    src/option_u64.c
    src/option_size.c
    src/option_f32.c
    src/option_f64.c
    src/option_obj.c
)

add_library(
    ${PROJECT_NAME} 
    ${option_SOURCES}
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/../headers
    ${CMAKE_CURRENT_SOURCE_DIR}/../munit
)
