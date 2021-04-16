cmake_minimum_required(VERSION 3.16)
project(matrix_mul_linux)

set(CMAKE_CXX_FLAGS -pthread)
set(CMAKE_CXX_STANDARD 17)

add_executable(matrix_mul_linux main.cpp matrices/matrix_operations.h matrices/rows_cols_threads_mul.h matrices/cols_rows_threads_mul.h matrices/block_threads_mul.h)