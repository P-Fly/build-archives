# Policies

**Policies** 是一种帮助 **CMake** 在不同版本之间实现兼容性的一种机制。

**cmake-policies** 的链接如下: https://cmake.org/cmake/help/latest/manual/cmake-policies.7.html#manual:cmake-policies(7)

## 设计目标

 - 构建现有项目使用的 **CMake** 应比项目作者的版本更新。

    - 用户不需要编辑代码就可以构建项目。

    - 虽然可能会发出警告，但项目仍应继续构建。

 - 新接口的正确性或旧接口的错误修复不应受到兼容性要求的限制。

 - 对 **CMake** 所做的每项改动 (可能需要更改 **CMakeLists** 文件) 都应记录下来。

    - 每个更改还应具有一个可通过警告和错误消息引用的唯一标识符。

    - 仅当项目以某种方式表明支持该新行为时，才会启用。

 - 最终必须能够删除实现与旧 **CMake** 版本兼容的代码。

    - 这种删除对于保持代码清洁和允许内部重构是必要的。

    - 删除后，尝试构建针对古代版本编写的项目必定会失败，并显示一条消息。

**CMake** 中的所有策略都以 **CMPNNNN** 格式分配一个名称，其中 **NNNN** 是一个整数值。

## 设置策略

项目可以针对每个策略进行设置，以请求旧行为或新行为。

 - 按照 **CMake** 的发行版本设置策略:

    ```
    cmake_policy(VERSION 3.20)

    or

    cmake_minimum_required(VERSION 3.20)
    ```

 - 单独设置策略

    ```
    cmake_policy(SET CMP0002 OLD)

    or

    cmake_policy(SET CMP0002 NEW)
    ```

## 策略栈

策略设置使用 **堆栈** 进行范围定义。当进入项目的新子目录时 (比如 **add_subdirectory**)，**堆栈** 会 **PUSH** 一个新的级别，并在离开时 **POP**。因此，在项目的一个目录中设置的策略不会影响父目录或兄弟目录，只会影响子目录。

用户可以使用 **cmake_policy** 命令 **PUSH** 和 **POP** 其自身的堆栈级别。

```
cmake_policy(PUSH)
cmake_policy(SET CMP0003 OLD)
add_executable(myexe ...)
cmake_policy(POP)
```

可以通过在命令行中运行命令来获取策略列表以及特定策略的帮助:

```
cmake --help-command cmake_policy
cmake --help-policies
cmake --help-policy CMP0003
```

## 支持多版本

 1. 在v2.8以上版本时，使用v2.8进行构建。否则使用v2.6进行构建:

    ```
    cmake_minimum_required(VERSION 2.6)
    if (NOT ${CMAKE_VERSION} VERSION_LESS 2.8)
       cmake_policy(VERSION 2.8)
    endif()
    ```

 2. 使用 **if** 命令检查新命令是否存在:

    ```
    if(COMMAND cmake_policy)
        cmake_policy(SET CMP0003 NEW)
    endif()
    ```

 3. 测试 **CMake** 的版本号:

    ```
    if(${CMAKE_VERSION} VERSION_GREATER 3.20)
        ...
    endif()
    ```
