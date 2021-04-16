cmake_minimum_required(VERSION 3.17)
project(matrix_mul_win)

set(CMAKE_CXX_STANDARD 17)

add_executable(matrix_mul main.cpp matrices/matrix_operations.h matrices/rows_cols_threads_mul.h matrices/cols_rows_threads_mul.h matrices/block_threads_mul.h)