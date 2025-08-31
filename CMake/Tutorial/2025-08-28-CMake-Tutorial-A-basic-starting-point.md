# A Basic Starting Point

https://cmake.org/cmake/help/latest/guide/tutorial/A%20Basic%20Starting%20Point.html

## 基本项目

 1. 编辑配置文件 **CMakeLists.txt** 生成标准的构建文件:

    - **cmake_minimum_required**: 任何项目最顶层的 **CMakeLists.txt** 都必须首先指定 **CMake** 的最低版本。

    - **project**: 设置项目名称。

    - **add_executable**: 使用指定的源代码文件创建可执行文件。

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial)

    add_executable(Tutorial tutorial.c)
    ```

 2. 构建并运行:

    ```
    cmake .
    cmke --build .
    ```

## 指定C++标准

**CMake** 有一些特殊变量，以 **CMAKE_** 开头。

```
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

## 添加配置参数

有时我们会在 **CMakeLists.txt** 中定义一个变量(比如版本号)，并在源码中使用这个变量。此时，可以使用 **configure_file**，对文件内容进行转换。

 1. 创建配置文件 **config.h.in**。

    ```
    #define MY_VERSION @VERSION@
    ```

 2. **CMakeLists.txt** 中设置版本号。

    ```
    set(VERSION 1)

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/config.h"
    )

    include_directories("${CMAKE_CURRENT_BINARY_DIR}")
    ```
