cmake_minimum_required(VERSION 3.16)
project(pipes_linux)

set(CMAKE_CXX_STANDARD 14)

add_executable(main main.cpp procs/help-funcs.h)
add_executable(mul-proc procs/mul-proc.cpp procs/help-funcs.h)
add_executable(add-proc procs/add-proc.cpp procs/help-funcs.h)
add_executable(pow-proc procs/pow-proc.cpp procs/help-funcs.h)
add_executable(sum-proc procs/sum-proc.cpp procs/help-funcs.h)
