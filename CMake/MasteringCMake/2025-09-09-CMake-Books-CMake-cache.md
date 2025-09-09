# CMake Cache

**CMake Cache** 是一个 **配置文件**。

首次在项目上运行 **CMake** 时，会在构建树的顶层目录中生成文件 **CMakeCache.txt**。

该文件存储一组 **全局缓存变量**，这些变量的值在项目构建树的多次运行中保持不变。

## 使用目的

 1. 存储自定义的 **用户配置**。

    ```
    option(USE_JPEG "Do you want to use the jpeg library")

    or

    set(USE_JPEG ON CACHE BOOL "include jpeg support?")
    ```

 2. 存储特定的 **系统相关变量**。

## 使用特点

 1. **CMake Cache** 可能会被持续构建多次，直到它不再发生变化 (类似于 **Makefile** 的 **include** 策略)。

 2. **CMake Cache** 不能直接编辑或者拷贝，因该由 **CMake** 自动生成。

 3. **CMake** 重复执行时，只有当 **CMake Cache** 中没有该变量时，命令才会生效。即如果修改了缓存值，一定记得要手动删除 **CMakeCache.txt**。

 4. 如果变量已经存在于 **CMake Cache** 中，可以在 **CMakeLists** 中使用 **set** 命令定义同名变量(参数不带 **CACHE**)。此时相当于定义了一个临时变量进行使用，并且不会修改缓存中的值。

    ```
    set(USE_JPEG ON)
    ```

 5. 如果希望 **CMake** 每次执行时都重新生成缓存变量的值，可以通过 **FORCE** 选项进行设置。

    ```
    set(USE_CACHE_2 ON CACHE BOOL "include jpeg support?" FORCE)
    ```

 6. 某些选项希望是默认隐藏的，只有选择高级选项时才能看见，可以通过 **mark_as_advanced** 进行设置。

    ```
    set(USE_JPEG ON CACHE BOOL "include jpeg support?")

    mark_as_advanced(USE_JPEG)
    ```

 7. 可以将 **CMake Cache** 的条目限制为一组有限的预定义选项。

    ```
    set(CRYPTOBACKEND "OpenSSL" CACHE STRING "Select a cryptography backend")
    set_property(CACHE CRYPTOBACKEND PROPERTY STRINGS "OpenSSL" "LibTomCrypt" "LibDES")
    ```

## 设置初始值

 1. 在命令行上使用 **\-DCACHE\_VAR:TYPE\=VALUE** 参数传递缓存值。

    ```
    cmake ../MyProject -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING:BOOL=ON
    ```

 2. 创建一个新的 CMake 文件，使用 -C 选项进行加载。

    ```
    cmake -C ../MyProject/config.cmake ../MyProject
    ```
