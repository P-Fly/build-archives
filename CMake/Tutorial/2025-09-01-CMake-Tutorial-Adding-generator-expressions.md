# Adding Generator Expressions

https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20Generator%20Expressions.html

## Generator Expressions

**Generator expressions** 在构建系统生成期间进行求值，以生成特定于每个构建配置的信息。其形式为 **\$\<...\>**。

**Generator expressions** 支持条件表达式，逻辑运算符，列表转换等。这些功能可以基于构建配置，目标属性，平台信息等定制不同的构建信息。

详细文档: https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html

```
cmake_minimum_required(VERSION 3.10)

project(Tutorial)

add_executable(Tutorial tutorial.cxx)

add_subdirectory(MathFunctions)
target_link_libraries(Tutorial PUBLIC MathFunctions)

# Generator Expressions
target_compile_options(Tutorial PUBLIC
    $<$<CXX_COMPILER_ID:GNU>:-Wall>
    $<$<CXX_COMPILER_ID:Clang>:-Weverything>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)
```
