cmake_minimum_required(VERSION 3.22.1)
project(${PROJECT_NAME} C)

add_subdirectory(tokenizer)
add_subdirectory(parser)
add_subdirectory(logger)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} 
    logger
    tokenizer
    parser
)

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/bin/
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)