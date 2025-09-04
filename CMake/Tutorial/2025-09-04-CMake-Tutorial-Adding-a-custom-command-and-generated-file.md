# Adding a Custom Command and Generated File

https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20a%20Custom%20Command%20and%20Generated%20File.html

## 自定义命令

可以使用 **add_custom_command()** 添加自定义命令，主要用于生成文件或执行特定操作。

详细文档: https://cmake.org/cmake/help/latest/command/add_custom_command.html

 1. 添加一个 **MakeTable.cmake** 的文件，用于描述自定义命令。

    ```
    add_executable(MakeTable MakeTable.cxx)

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
        COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
        DEPENDS MakeTable
        )
    ```

 2. 在顶级配置文件 **CMakeLists.txt** 中，引用 **MakeTable.cmake**。

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial VERSION 1.0)

    add_executable(Tutorial main.c ${CMAKE_CURRENT_BINARY_DIR}/Table.h)

    include(MakeTable.cmake)
    ```
