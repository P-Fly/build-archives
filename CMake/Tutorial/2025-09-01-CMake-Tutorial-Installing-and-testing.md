# Installing and Testing

https://cmake.org/cmake/help/latest/guide/tutorial/Installing%20and%20Testing.html

## 安装

可以使用 **install()** 指定安装规则，通常只需指定安装位置和要安装的目标和文件。

详细文档: https://cmake.org/cmake/help/latest/command/install.html

 1. 编辑配置文件 **CMakeLists.txt**，添加库的安装命令:

    ```
    cmake_minimum_required(VERSION 3.10)

    project(testlib)

    add_library(testlib lib_src/lib1.c lib_src/lib2.c)

    target_include_directories(testlib PUBLIC
                               "${PROJECT_BINARY_DIR}"
                               "lib_inc"
                              )

    install(TARGETS testlib DESTINATION lib)
    install(FILES lib_inc/lib.h DESTINATION include)
    ```

 2. 在顶级配置文件 **CMakeLists.txt** 中，添加安装命令:

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial VERSION 1.0)

    add_subdirectory(lib_source)

    add_executable(Tutorial main.c)

    target_link_libraries(Tutorial PUBLIC testlib)

    install(TARGETS Tutorial DESTINATION bin)
    ```

 3. 运行

    ```
    # 安装至默认位置 /usr/local/
    make install
    or
    cmake --install .

    # 定义变量 CMAKE_INSTALL_PREFIX 或使用 --install--prefix 用于指定将要安装的位置
    cmake --install . --prefix "/home/pengfei24/workspace/test/release/"
    ```
