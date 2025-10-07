# System Inspection

## 搜索头文件和库

此类搜索功能最相关的命令是 **find_file**，**find_library**，**find_path**，**find_program** 和 **find_package**。

```
# find libtiff, looking in some standard places
find_library(TIFF_LIBRARY
             NAMES tiff tiff2
             PATHS /usr/local/lib /usr/lib
            )

# find tiff.h looking in some standard places
find_path(TIFF_INCLUDES tiff.h
           /usr/local/include
           /usr/include
          )

add_executable(mytiff mytiff.c )

target_link_libraries(mytiff ${TIFF_LIBRARY})

target_include_directories(mytiff ${TIFF_INCLUDES})
```

## 系统属性

为了获得最大的可移植性，软件不应针对特定平台进行预处理，而应针对特定功能进行预处理。

```
// 推荐做法: Coding to a feature
#ifdef HAS_FOOBAR_CALL
  foobar();
#else
  myfoobar();
#endif

// 不推荐: Coding to specific platforms
#if defined(SUN) && defined(HPUX) && !defined(GNUC)
  foobar();
#else
  myfoobar();
#endif
```

### 使用 try_compile 和 try_run 测试系统属性

[try_compile And try_run Demo][1]

### 预定义的 try_compile 和 try_run 模块

 - **CheckIncludeFile**: 用于检查是否包含指定的 **C** 头文件。

 - **CheckIncludeFileCXX**: 用于检查是否包含指定的 **C\+\+** 头文件。

 - **CheckIncludeFiles**: 用于检查给定的头文件是否可以一起包含。

 - **CheckLibraryExists**: 用于检查某个库是否存在。

 - **CheckSymbolExists**: 用于检查某个符号是否在头文件中定义。

 - **CheckTypeSize**: 用于确定变量类型的字节大小。

 - **CheckVariableExists**: 用于检查全局变量是否存在。

## 配置软件

### 编译行中传递宏定义

```
if (HAS_FOOBAR_CALL)
    target_compile_definitions(mytarget PUBLIC HAS_FOOBAR_CALL)
endif()
```

### 通过头文件传递宏定义

```
# ---- CMakeLists.txt file-----

# Configure a file from the source tree
# called projectConfigure.h.in and put
# the resulting configured file in the build
# tree and call it projectConfigure.h
configure_file(
   ${PROJECT_SOURCE_DIR}/projectConfigure.h.in
   ${PROJECT_BINARY_DIR}/projectConfigure.h
   @ONLY
   )

# ---- projectConfigure.h.in file-----

/* define a variable to tell the code if the */
/* foobar call is available on this system */
#cmakedefine HAS_FOOBAR_CALL

/* define a variable with the path to the */
/* build directory  */
#define PROJECT_BINARY_DIR "@PROJECT_BINARY_DIR@"
```

 [1]: https://github.com/P-Fly/build-archives/tree/master/CMake/MasteringCMake/demo/SystemInspection/try-demo
