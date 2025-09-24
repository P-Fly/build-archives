cmake_minimum_required(VERSION 3.20)
project(MyMainProject C)

find_package(gromit 1.0 REQUIRED)
include_directories(${gromit_INCLUDE_DIRS})

add_executable(main main.c)
target_link_libraries(main gromit)
