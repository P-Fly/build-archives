# Adding Usage Requirements for a Library

https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Usage%20Requirements%20for%20a%20Library.html

## 为库添加使用要求

 1. 编辑配置文件 **CMakeLists.txt**，定制库文件的配置要求 (下例中，头文件为 **INTERFACE**):

    ```
    add_library(MathFunctions mysqrt.cxx)

    target_include_directories(MathFunctions
                               INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                               )
    ```

 2. 在顶级配置文件 **CMakeLists.txt** 中，添加库依赖。

    ```
    cmake_minimum_required(VERSION 3.10)

    project(Tutorial VERSION 1.0)

    option(USE_MYMATH "Use tutorial provided math implementation" ON)

    if(USE_MYMATH)
        add_subdirectory(MathFunctions)
        list(APPEND EXTRA_LIBS MathFunctions)
    endif()

    add_executable(Tutorial tutorial.cxx)

    target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

    target_include_directories(Tutorial PUBLIC
                               "${PROJECT_BINARY_DIR}"
                              )
    ```

## 通过接口库设置 C++ 标准

```
cmake_minimum_required(VERSION 3.10)

project(Tutorial)

add_executable(Tutorial tutorial.cxx)

add_subdirectory(MathFunctions)
target_link_libraries(Tutorial PUBLIC MathFunctions)

# 定义一个接口库 tutorial_compiler_flags，该库不包含任何源码文件
add_library(tutorial_compiler_flags INTERFACE)

# 添加编译器功能: 因为是 INTERFACE，所以依赖 tutorial_compiler_flags 的目标都会加载 cxx_std_11
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)

# 将 tutorial_compiler_flags 链接到 Tutorial
target_link_libraries(Tutorial PUBLIC tutorial_compiler_flags)
```

## Public / Interface / Private之间的区别

![Visibility Model][1]

 1. **Public**: 使用 **PUBLIC** 可见性时，相关属性会同时应用于目标自身和所有使用该目标的消费者。

    ```
    target_include_directories(mylib PUBLIC
        include/public  # 所有使用 mylib 的目标都需要这些头文件
    )

    target_link_libraries(mylib PUBLIC
        dependency_lib  # 将 dependency_lib 的依赖传递给使用 mylib 的目标
    )
    ```

    适用场景：

    - 当依赖是接口的一部分时。

    - 如果库的公共头文件包含其他库的头文件，并且使用者需要知道该依赖的存在。

    - 当依赖提供必要的运行时功能时，如果没有这些依赖，使用者的代码将无法运行。

 2. **Private**: 使用 **PRIVATE** 可见性时，相关属性仅应用于目标自身，不会传播给消费者。

    ```
    target_include_directories(mylib PRIVATE
        src/internal
    )

    target_link_libraries(mylib PRIVATE
        internal_helper_lib
    )
    ```

    适用场景：

    - 需要隐藏实现细节所需的内部头文件。

    - 库的内部实现所需，但不暴露给外部的依赖。

    - 纯实现层面使用的工具库。

 3. **Interface**: 使用 **INTERFACE** 可见性时，相关属性不应用于目标自身，只传播给消费者。

    ```
    target_include_directories(header_only_lib INTERFACE
        include
    )

    target_link_libraries(header_only_lib INTERFACE
        required_dependency
    )
    ```

    适用场景：

    - 纯头文件的模板库。

    - 使用者需要的编译标志或定义。

    - 接口库，仅提供编译/链接需求。

 [1]: ./images/visibility_model.jpg
