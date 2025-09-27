# Converting Existing Systems To CMake

## Source Code Directory Structures

建议在顶层目录创建一个 **CMakeLists** 文件，然后使用 **add_subdirectory** 命令进入并管理子目录。

对于源代码位于多个子目录中的项目，有 3 种方案可供选择:

 1. 在顶层目录的 **CMakeLists** 文件中，列出其子目录中所有需要编译的源文件。

	```
	set(SOURCES
		subdir1/foo.cxx
		subdir2/bar.cxx
		...
	)
	```

 2. 将每个子目录构建成一个或多个库，然后将其链接到可执行文件中。

	```
	add_subdirectory(foo)
	add_subdirectory(bar)

	target_link_libraries(main foo bar)
	```

3. 前两种方法的混合，即顶级 **CMakeLists** 文件使用 **include** 命令包含每个子目录，然后在每个子目录的 **CMakeLists** 文件中列出源文件。

	```
	# 顶层 CMakeLists
	include(subdir1/CMakeLists.txt)
	foreach(FILE ${FILES})
		set(subdir1Files ${subdir1Files} subdir1/${FILE})
	endforeach()

	include(subdir2/CMakeLists.txt)
	foreach(FILE ${FILES})
		set(subdir2Files ${subdir2Files} subdir2/${FILE})
	endforeach()

	add_executable(main ${subdir1Files} ${subdir2Files})

	# 子目录 CMakeLists
	set(FILES 
		foo1.cxx
		foo2.cxx
	)
	```

## Build Directories

构建目录一般是建议将二进制文件放入与源代码树结构相同的单独树中。

如果需要从一个源树支持多种体系结构，建议构建目录的结构如下:

```
projectfoo/
  foo/
    subdir1
    subdir2
  foo-linux/
    subdir1
    subdir2
  foo-osx/
    subdir1
    subdir2
  foo-solaris/
    subdir1
    subdir2
```

**CMake** 提供了三个变量来控制二进制目标的写入位置:

 - **CMAKE_RUNTIME_OUTPUT_DIRECTORY**
 
 - **CMAKE_LIBRARY_OUTPUT_DIRECTORY**
 
 - **CMAKE_ARCHIVE_OUTPUT_DIRECTORY**

```
# Setup output directories.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)	
```
