cmake_minimum_required(VERSION 3.0)
project(rns)

add_executable(
    ${PROJECT_NAME} 
    src/main.c
    src/settings.c
)
target_link_libraries(${PROJECT_NAME} PUBLIC interface vector option avltreeset)
