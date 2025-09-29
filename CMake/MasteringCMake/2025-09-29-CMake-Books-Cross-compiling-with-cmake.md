# Cross Compiling With CMake

 - **Build Host**: 用于构建软件的系统。

 - **Target System / Target Platform**: 软件所构建的目标系统。

交叉编译对 **CMake** 有几个影响:

 - 无法自动检测目标平台。

 - 无法在默认系统目录中找到库和头文件。

 - 交叉编译期间构建的可执行文件无法执行。

## Toolchain Files

```
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_C_COMPILER arm-linux-gnueabi-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabi-g++)

set(CMAKE_FIND_ROOT_PATH /home/user/.install)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
```

构建命令如下:

```
cmake -DCMAKE_TOOLCHAIN_FILE=../source/toolchain.cmake ../source/
```

 - **CMAKE_TOOLCHAIN_FILE**: 指定 **Toolchain File**。

 - **CMAKE_SYSTEM_NAME**:

    - 此变量是必需的，它设置 **Target System** 的名称。典型名称为 **Linux** 和 **Windows**，如果目标系统没有操作系统，则该名称为 **Generic**。

    - 以这种方式预设 **CMAKE_SYSTEM_NAME** 会导致 **CMake** 自动将构建视为 **交叉编译构建**，此时 **CMAKE_CROSSCOMPILING** 将被设置为 **TRUE**。

 - **CMAKE_SYSTEM_VERSION**: 设置 **Target System** 的版本。

 - **CMAKE_SYSTEM_PROCESSOR**:

    - 此变量是可选的，它设置 **Target System** 的处理器或硬件名称。用于加载 **\$\{CMAKE_SYSTEM_NAME\}\-COMPILER\_ID\-\$\{CMAKE_SYSTEM_PROCESSOR\}.cmake**。

    - 此文件可用于修改目标的编译器标志等设置。

    - 只有在使用交叉编译器时才需要设置此变量，因为每个目标都需要特殊的构建设置。

 - **CMAKE_C_COMPILER**: 指定 **C** 编译器可执行文件的完整路径或文件名。

 - **CMAKE_CXX_COMPILER**: 指定 **C++** 编译器可执行文件的完整路径或文件名。

## Finding External Libraries, Programs and Other Files

**CMake** 使用 **find_program**，**find_library**，**find_file**，**find_path** 和 **find_package** 命令用于搜索 **外部库** 或 **工具**。

其中 **find_package** 有点不同，它实际上是执行 **Find\<\*\>.cmake** 模块，然后调用 **find_program**，**find_library**，**find_file** 和 **find_path** 命令。

在交叉编译环境下，您需要查找适用于 **Target System** 的文件，在其他情况下，您需要查找 **Build Host** 的文件。

 - **CMAKE_FIND_ROOT_PATH**: 包含 **Target System** 的目录列表。此处列出的每个目录前缀都会添加到每个 **find** 命令的搜索目录之前。默认情况下，该值为空。

 - **CMAKE_FIND_ROOT_PATH_MODE_PROGRAM**: 设置命令的搜索模式。

    - **NEVER**: 不使用 **CMAKE_FIND_ROOT_PATH** 设置的目录前缀进行搜索。

    - **ONLY**: 只使用 **CMAKE_FIND_ROOT_PATH** 设置的目录前缀进行搜索。

    - **BOTH**: 首先使用 **CMAKE_FIND_ROOT_PATH** 设置的目录前缀进行搜索，然后搜索不带前缀的目录。

 - **CMAKE_FIND_ROOT_PATH_MODE_LIBRARY**: 设置库的搜索模式。

 - **CMAKE_FIND_ROOT_PATH_MODE_INCLUDE**: 设置头文件的搜索模式。

## System Inspection

 - 一般用变量 **CMAKE_HOST_XXX** 来描述 **Build Host**。

 - 一般用变量 **CMAKE_SYSTEM_XXX** 来描述 **Target System**。

```
if(CMAKE_SYSTEM MATCHES Windows)
   message(STATUS "Target system is Windows")
endif()

if(CMAKE_HOST_SYSTEM MATCHES Linux)
   message(STATUS "Build host runs Linux")
endif()
```

## Running Executables Built in the Project

典型应用: 在 **Build Host** 中编译工具，并导出配置文件。在 **Target System** 中使用此 **Package**。

```
if(CMAKE_CROSSCOMPILING)
   find_package(MyGen)
else()
   add_executable(mygen gen.c)
   export(TARGETS mygen FILE
          "${CMAKE_BINARY_DIR}/MyGenConfig.cmake")
endif()

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/generated.h"
  COMMAND mygen -o "${CMAKE_CURRENT_BINARY_DIR}/generated.h"
  )
```
