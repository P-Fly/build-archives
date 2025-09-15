# Key Concepts

## Property

许多 **CMake** 对象都具有与其关联的 **属性**，可以通过 **set_property** 和 **get_property** 命令进行访问。

也可以使用 **set_target_properties** 和 **get_target_property** 命令进行访问。

完整的属性列表如下: https://cmake.org/cmake/help/latest/manual/cmake-properties.7.html#manual:cmake-properties(7)

## Targets

**Targets** 常常代表构建时生成的 **可执行文件**，**库** 和 **实用程序**。

 - **add_executable**

    ```
    add_executable(main main.cxx)
    target_link_libraries(main foo)
    ```

 - **add_library**

    ```
    add_library(foo STATIC foo.cxx)
    target_link_libraries(foo bar)
    target_include_directories(foo PUBLIC
                               "public_inc"
                              )
    target_include_directories(foo PRIVATE
                               "private_inc"
                              )
    ```

 - **add_custom_target**

**CMake** 在 **Windows** 平台上，支持根据不同的编译配置，链接不同的库。

```
add_executable(foo foo.c)
# 如果是 Debug 模式则链接 libdebug, 如果是 optimized 模式则链接 libopt.
target_link_libraries(foo debug libdebug optimized libopt)
```

## Object Libraries

**Object Libraries** 是编译成目标文件的源文件的集合。

有时候，希望这些文件不自动链接到 **library** 或 **archive** 中，而是希望单独的合并或是处理。这时，可以使用 **\$\<TARGET\_OBJECTS:name\>** 作为源来引用。

```
# Combined 为包含 A 和 B 的库。 注意: 如果 A 和 B 定义了头文件依赖，不会被 Combined 自动继承.
add_library(Combined $<TARGET_OBJECTS:A> $<TARGET_OBJECTS:B>)
```

## Source Files

可以使用 **set_source_files_properties** 和 **get_source_file_property** 设置和获取文件属性。

## Directories, Tests, and Properties

可以使用 **set_directory_properties** 和 **set_tests_properties** 设置这些属性。
