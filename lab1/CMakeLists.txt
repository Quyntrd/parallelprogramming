cmake_minimum_required(VERSION 3.11)

project(Matrix LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CUDA_STANDARD 11)

add_library(Matrix SHARED
    include/Matrix.h
    src/Matrix.cu
)

target_include_directories(Matrix PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

set_target_properties(Matrix PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
)

add_executable(MatrixExe
    src/MatrixTests.cc
)

set_source_files_properties(src/MatrixTests.cc PROPERTIES LANGUAGE CUDA)

target_link_libraries(MatrixExe PRIVATE Matrix)
