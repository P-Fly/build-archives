# Modules

**Modules** 的使用是为了代码复用，详细的介绍参见: https://cmake.org/cmake/help/latest/manual/cmake-modules.7.html#manual:cmake-modules(7)

**Modules** 的默认位置一般为 **/usr/share/cmake-\<version\>/Modules/**。

也可以通过 **CMAKE_MODULE_PATH** 指定查找目录。

**Modules** 可以分为以下两个主要类别:

 1. **查找模块**

    支持 **find_package** 命令用于查找软件包的位置。

 2. **实用模块**

    可以使用 **include** 命令，将其插入到 **CMakeLists** 中。
