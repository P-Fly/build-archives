cmake_minimum_required(VERSION 3.20)
project(main C)

add_executable(main main.c)

file(WRITE
     ${CMAKE_BINARY_DIR}/testCCompiler.c
     "int main(){return 0;}"
)

try_compile(COMPILE_RESULT_VAR
            ${CMAKE_BINARY_DIR}
            ${CMAKE_BINARY_DIR}/testCCompiler.c
            OUTPUT_VARIABLE OUTPUT
)

if(COMPILE_RESULT_VAR)
    target_compile_definitions(main PRIVATE CONFIG_C_COMPILER="Yes")
else()
    target_compile_definitions(main PRIVATE CONFIG_C_COMPILER="No")
endif()

try_run(RUN_RESULT_VAR
        COMPILE_RESULT_VAR
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/TestByteOrder.c
        OUTPUT_VARIABLE OUTPUT
)

if(COMPILE_RESULT_VAR EQUAL 1)
    target_compile_definitions(main PRIVATE CONFIG_BYTE_ORDER="Big-Endian")
else()
    target_compile_definitions(main PRIVATE CONFIG_BYTE_ORDER="Little-Endian")
endif()
