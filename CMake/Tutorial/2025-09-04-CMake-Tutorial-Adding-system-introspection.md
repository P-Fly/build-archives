# Adding System Introspection

https://cmake.org/cmake/help/latest/guide/tutorial/Adding%20System%20Introspection.html

## 系统自检

有时我们需要检查当前的目标平台是否具备一些功能可用，此时可以使用 **CheckCXXSourceCompiles模块** 来检测。

该模块在真正的源代码编译之前会首先尝试编译具有所需依赖项的简单代码，然后根据编译结果确定该依赖项是否可用。

```
include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
    #include <cmath>
    int main() {
      std::log(1.0);
      return 0;
    }"
    CONFIG_LOG)

if(CONFIG_LOG)
    target_compile_definitions(Tutorial PRIVATE "CONFIG_LOG")
endif()
```
