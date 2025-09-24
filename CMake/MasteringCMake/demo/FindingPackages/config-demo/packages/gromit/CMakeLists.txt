cmake_minimum_required(VERSION 3.20)
project(Gromit C)
set(version 1.0)

add_library(gromit STATIC gromit.c gromit.h)

install(FILES gromit.h 
		DESTINATION include/gromit-${version})
install(TARGETS gromit 
        DESTINATION lib/gromit-${version}
        EXPORT gromit-target)
install(EXPORT gromit-target
		DESTINATION lib/gromit-${version})

configure_file(
   ${Gromit_SOURCE_DIR}/gromit-config.cmake.in
   ${Gromit_BINARY_DIR}/gromit-config.cmake @ONLY)

configure_file(
   ${Gromit_SOURCE_DIR}/gromit-config-version.cmake.in
   ${Gromit_BINARY_DIR}/gromit-config-version.cmake @ONLY)

install(FILES ${Gromit_BINARY_DIR}/gromit-config.cmake
         ${Gromit_BINARY_DIR}/gromit-config-version.cmake
         DESTINATION lib/gromit-${version})
