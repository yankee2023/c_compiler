cmake_minimum_required(VERSION 3.22.1)
project(${PROJECT_NAME} C)

add_subdirectory(logger)
add_subdirectory(tokenizer)
add_subdirectory(ast)
add_subdirectory(assembler)

add_executable(${PROJECT_NAME} main.c)

target_link_libraries(${PROJECT_NAME} 
    logger
    tokenizer
    ast
    assembler
)

add_custom_command(
    TARGET ${PROJECT_NAME}                      # ファイルをコピーする際に依存するターゲット
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/src/${PROJECT_NAME} # コピー元のファイルパス
        ${CMAKE_SOURCE_DIR}/bin/                # コピー先のファイルパス
)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)