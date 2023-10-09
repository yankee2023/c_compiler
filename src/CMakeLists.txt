cmake_minimum_required(VERSION 3.22.1)
project(${PROJECT_NAME} C)

add_subdirectory(tokenizer)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} tokenizer)
target_link_libraries(${PROJECT_NAME} parser)
target_link_libraries(${PROJECT_NAME} logger)