cmake_minimum_required(VERSION 3.22.1)
project(${PROJECT_NAME} C)

set(CMAKE_C_FLAG "-Wall")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # デバッグビルド用のオプションを設定
    add_compile_options(-g -O0)  # -gはデバッグ情報を含めるため、-O0は最適化を無効化
else()
    # リリースビルド用のオプションを設定
    add_compile_options(-O3)  # 最適化を有効にする
endif()

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