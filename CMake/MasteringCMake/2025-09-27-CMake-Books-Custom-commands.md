# Custom Commands

**CMake** 使用 **add_custom_command** 和 **add_custom_target** 命令来支持构建过程中或构建之后可能需要执行的各种任务。

## Portable Custom Commands

**add_custom_command** 命令通常是以文件作为输入或输出的程序，**CMake** 提供了三个方法来处理不同平台之间的差异:

 - 使用 **-E option** 指示 **CMake** 按照独立于平台的方式来执行命令。

    ```
    add_custom_command(
      TARGET Foo
      POST_BUILD
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy $<TARGET_FILE:Foo> /testing_department/files
      )
    ```

 - 通过一些描述平台特征的变量来解决可移植性的问题，这些变量包括 **CMAKE_EXECUTABLE_SUFFIX**，**CMAKE_SHARED_LIBRARY_PREFIX** 等。

 - 支持 **generator expressions**，这些表达式在构建系统生成时才进行求值。

## Add Custom Command

```
add_executable(Foo bar.c)

add_custom_command(
  TARGET Foo
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:Foo> /testing_department/files
)
```

## Generate a File

```
add_executable(creator creator.cxx)

add_custom_command(
  OUTPUT ${PROJECT_BINARY_DIR}/created.c
  DEPENDS creator
  COMMAND creator ${PROJECT_BINARY_DIR}/created.c
)

add_executable(Foo ${PROJECT_BINARY_DIR}/created.c)
```

## Adding a Custom Target

```
add_custom_command(
  OUTPUT  ${PROJECT_BINARY_DIR}/doc1.dvi
  DEPENDS ${PROJECT_SOURCE_DIR}/doc1.tex
  COMMAND ${LATEX} ${PROJECT_SOURCE_DIR}/doc1.tex
)

add_custom_command(
  OUTPUT  ${PROJECT_BINARY_DIR}/doc1.pdf
  DEPENDS ${PROJECT_BINARY_DIR}/doc1.dvi
  COMMAND ${DVIPDF} ${PROJECT_BINARY_DIR}/doc1.dvi
)

add_custom_target(TDocument ALL
  DEPENDS ${PROJECT_BINARY_DIR}/doc1.pdf
)
```

## Specifying Dependencies and Outputs

指定依赖项或自定义命令的输出时，应始终指定 **完整路径**。
