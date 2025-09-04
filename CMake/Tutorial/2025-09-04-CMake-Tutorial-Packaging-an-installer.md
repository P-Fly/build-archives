# Packaging an Installer

https://cmake.org/cmake/help/latest/guide/tutorial/Packaging%20an%20Installer.html

## 打包

如果我们想将我们的项目分发给其他人，可以使用 **cpack** 构建支持二进制安装和包管理功能的安装包。

 - **InstallRequiredSystemLibraries**: 此模块将包含项目在当前平台上所需的任何运行时库。

 - **CPACK_RESOURCE_FILE_LICENSE**: 记录许可证文件位置。

 - **CPACK_PACKAGE_VERSION_MAJOR** / **CPACK_PACKAGE_VERSION_MINOR**: 记录版本信息。

 - **CPACK_GENERATOR**: 用于二进制生成的工具。

 - **CPACK_SOURCE_GENERATOR**: 用于源码生成的工具。

 1. 编辑配置文件 **CMakeLists.txt**:

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial VERSION 1.0)

    add_executable(Tutorial main.c ${CMAKE_CURRENT_BINARY_DIR}/Table.h)

    include(MakeTable.cmake)

    install(TARGETS Tutorial DESTINATION bin)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Table.h DESTINATION include)

    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
    set(CPACK_PACKAGE_VERSION_MAJOR "1")
    set(CPACK_PACKAGE_VERSION_MINOR "2")
    set(CPACK_GENERATOR "TGZ")
    set(CPACK_SOURCE_GENERATOR "TGZ")
    include(CPack)
    ```

 2. 构建安装包:

    ```
    # 构建二进制分发包
    cpack

    # 创建源代码树存档
    cpack --config CPackSourceConfig.cmake
    ```
