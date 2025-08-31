# Adding a Library

https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20a%20Library.html

## 创建一个库

 1. 编辑配置文件 **CMakeLists.txt**，生成库的构建文件:

    - **add_library**: 添加一个库目标。

    - **target_include_directories**: 指定编译给定目标时使用的包含目录。

    ```
    cmake_minimum_required(VERSION 3.10)

    project(testlib)

    add_library(testlib lib_src/lib1.c lib_src/lib2.c)

    target_include_directories(testlib PUBLIC
                              "${PROJECT_BINARY_DIR}"
                              "lib_inc"
                              )
    ```

 2. 在顶级配置文件 **CMakeLists.txt** 中，添加一个子目录，以构建库。

    - **add_subdirectory**: 增加一个编译子目录。

    - **target_link_libraries**: 指定在链接给定目标或其依赖项时，需要使用的库。

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial VERSION 1.0)

    add_subdirectory(lib_source)

    add_executable(Tutorial main.c)

    target_link_libraries(Tutorial PUBLIC testlib)
    ```

## 添加选项

 1. 添加一个配置项

    ```
    option(CONFIG_USE "Use configure" ON)

    if (CONFIG_USE)
        target_compile_definitions(testlib PRIVATE "CONFIG_USE")
    endif()
    ```

 2. 构建并运行:

    ```
    cmake ../Step2 -DCONFIG_USE=OFF
    cmake --build .
    ```

 3. 代码中使用:

    ```
    #ifdef CONFIG_USE
        printf("definition CONFIG_USE.\n");
    #else
        printf("No definition CONFIG_USE.\n");
    #endif
    ```
