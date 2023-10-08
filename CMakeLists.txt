cmake_minimum_required(VERSION 3.22.1)
project(c_compiler C)

# in-source ビルドガード
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
endif()

include_directories(DIRECTORY ${CMAKE_SOURCE_DIR}/include)

# 以下のフォルダにあるCMakeLists.txtも読み込んでビルドターゲットを生成する
add_subdirectory(src)