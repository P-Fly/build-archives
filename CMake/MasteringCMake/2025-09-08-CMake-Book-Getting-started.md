# Getting Started

## Directory Structure

**CMake** 在构建项目时使用两个主要目录: **源目录** 和 **二进制目录**。

 - **源目录** 是项目源代码所在的位置，也是 **CMakeLists** 文件所在的位置。

 - **二进制目录** 也称为 **构建目录**，在其中放置生成的目标文件，库和可执行文件。

**CMake** 不会将任何文件写入 **源目录**，只会写入 **二进制目录**。

建议使用源码外构建，即源代码和二进制文件目录不同，这用当使用不同选项进行多次构建时，只需要一份源代码副本。

## Basic CMake Usage

**CMake** 将一个或多个 **CMakeLists** 文件作为输入，并生成 **项目文件** 或 **Makefile**。典型流程如下:

 1. 在项目中定义一个或多个 **CMakeLists** 文件。

 2. 使用 **CMake** 配置并生成项目。

 3. 使用本地开发工具构建项目。

## CMakeLists Files

```
# 定义项目需要的CMake版本，允许向后兼容
cmake_minimum_required(VERSION 3.20)

# 设置项目的名称，并可以指定其它选项
project(Hello)

# 将给定的源文件添加到可执行文件的依赖中
add_executable(Hello Hello.c)
```

**CMakeLists** 采用 **CMake Language** 编写，官网如下:

https://cmake.org/cmake/help/latest/manual/cmake-language.7.html#manual:cmake-language(7)

对于项目中的每个目录，**CMakeLists.txt** 文件会根据 **project** 生成一个顶层 **Makefile** 文件。该项目会包含 **CMakeLists.txt** 和子目录中通过 **add_subdirectory** 定义的所有目标。如果 **EXCLUDE_FROM_ALL** 被使用，该目标将不会出现在顶层 **Makefile** 中。这对于生成与主构建过程无关的子项目非常有用。 比如某些工具，不需要在每次构建时都重新构建这部分工具。

```
set_property(TARGET ${target} PROPERTY EXCLUDE_FROM_ALL TRUE)
```

## Configure and Generate

### Running CMake

 1. Running the CMake GUI: 基于 Qt 的应用程序，可以在大多数平台上使用。

 2. Running the ccmake Curses Interface: 基于终端的文本应用程序，与 **CMake GUI** 相似。

 3. Running CMake from the Command Line: 基于命令行的工具，适用于没有或只有很少可选项的项目。

### Specifying the Compiler to CMake

可以向 **CMake** 指定所需编译器的位置:

 1. The generator can specify the compiler.

 2. An environment variable can be set.

    ```
    CC = xxx
    CFLAGS = xxx

    CXX = xxx
    CXXFLAGS = xxx
    ```

 3. A cache entry can be set.

    ```
    -DCMAKE_C_COMPILER=xxx
    -DCMAKE_C_FLAGS=xxx

    -DCMAKE_CXX_COMPILER=xxx
    -DCMAKE_CXX_FLAGS=xxx
    ```

### Build Configurations

构建配置允许对同一份源码以不同的方式进行构建。以适应调试、优化或任何其他特殊标志集。

 - **Debug**: 启用了基本的调试标志。

 - **Release**: 启用了基本的优化。

 - **MinSizeRel**: 可以生成最小的目标代码，但不一定是最快的代码。

 - **RelWithDebInfo**: 构建包含调试信息的优化版本。

可以使用 **CMAKE_BUILD_TYPE** 指定具体的配置项，此时会将相应的变量和规则添加到编译行中 (例如 **CMAKE_CXX_FLAGS_\<ConfigName\>**)。

```
ccmake ../MyProject -DCMAKE_BUILD_TYPE=Debug

ccmake ../MyProject -DCMAKE_BUILD_TYPE=Release
```
