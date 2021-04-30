cmake_minimum_required(VERSION 3.16)
project(procs)

set(CMAKE_CXX_STANDARD 17)

add_executable(user user.cpp)
add_executable(killer killer.cpp)