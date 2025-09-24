cmake_minimum_required(VERSION 3.20)
project(Gromit C)
set(version 1.0)

add_library(gromit STATIC gromit.c gromit.h)

configure_file(
   ${Gromit_SOURCE_DIR}/Findgromit.cmake.in
   ${Gromit_BINARY_DIR}/Findgromit.cmake @ONLY)

install(FILES gromit.h
        DESTINATION include/gromit-${version})
install(TARGETS gromit
        DESTINATION lib/gromit-${version})
install(FILES ${Gromit_BINARY_DIR}/Findgromit.cmake
        DESTINATION ${CMAKE_MODULE_PATH}
)
