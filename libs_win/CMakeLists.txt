cmake_minimum_required(VERSION 3.17)
project(os_libs_win)
set(CMAKE_CXX_STANDARD 17)

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/Number
        ${CMAKE_CURRENT_SOURCE_DIR}/Vector
)


set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(number STATIC Number/number.h Number/number.cpp)

add_library(vector SHARED Vector/vector.cpp Vector/vector.h)
target_include_directories(vector PUBLIC ${CMAKE_SOURCE_DIR}/Number)


add_executable(Main main.cpp)

target_link_libraries(Main PUBLIC number)
target_link_libraries(vector PRIVATE number)
target_link_libraries(Main PUBLIC vector)