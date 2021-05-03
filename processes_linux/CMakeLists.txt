cmake_minimum_required(VERSION 3.16)
project(procs)

set(CMAKE_CXX_STANDARD 14)

add_executable(user user.cpp help_functions.h)
add_executable(killer killer.cpp help_functions.h)