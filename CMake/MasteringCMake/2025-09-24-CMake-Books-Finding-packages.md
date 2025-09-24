# Finding Packages

**CMake** 项目可以使用 **find_package** 命令来查找所依赖的外部软件包。

```
find_package(<Package> [version])
```

 - **Package**: 待查找软件包的名称。

 - **version**: 可选的版本请求，格式为 **major.minor.patch**。

## 运行模式

### Module mode

在该模式下，通过 **Find\<Package\>.cmake** 文件来查找包。

**find_package** 查找顺序为首先查找 **CMAKE_MODULE_PATH** 指定的路径，然后在 **CMake** 默认的安装路径 **/usr/share/cmake-\<version\>/Modules/** 中查找。

### Config mode

在该模式下，**find_package** 会自动搜索 **package configuration file**。即一个由外部软件包提供，名字为 **\<Package\>Config.cmake** 或 **\<package\>-config.cmake** 的文件。

**find_package** 查找路径为 **\<prefix\>/lib/\<package\>/\<package\>-config.cmake**，其中 **prefix** 部分可以使用 **CMAKE_PREFIX_PATH** 指定。

找到该软件包时，**CMake** 会创建一个名为 **\<Package\>_DIR** 的 **cache entry** 来存储软件包的位置。

一旦找到了该软件包的配置文件，则该配置文件会提供该软件包组件内容的位置，无需任何额外的搜索。

### 对比

| | Module mode | Config mode |
| :-: | :-: | :-: |
| 模块文件的实现方式 | 通过查找 **Find\<Package\>.cmake** 文件来实现 | 通过查找 **\<package\>-config.cmake** 文件来实现 |
| 模块文件的路径 | 模块文件通常位于 **CMake** 的模块路径 | 模块文件通常存放在模块的安装路径 |
| 模块文件的提供者 | 由包的使用者提供，可能包含复杂的查找逻辑 | 由包的作者提供，通常更简洁 |
| 模块适用的场景 | 适用于没有提供 **CMake** 配置文件的第三方库 | 适用于提供了 **CMake** 配置文件的库 |

## Built-in Find Modules

**CMake** 有许多预定义模块，可以在 **/usr/share/cmake-\<version\>/Modules/** 的子目录中找到，这些模块可以找到许多常用的软件包。

每个 **Find\<XX\>.cmake** 模块都定义了一组变量，允许项目在找到软件包后使用它，推荐的默认变量约定如下(实际上一些预定义模块有许多自己定义的变量):

 - **\<XX\>_INCLUDE_DIRS**: 头文件的路径。

 - **\<XX\>_LIBRARIES**: 要链接使用的库，这些包含完整路径。

 - **\<XX\>_DEFINITIONS**: 编译使用 **\<XX\>** 的代码时使用的预处理器定义。

 - **\<XX\>_EXECUTABLE**: 在哪里可以找到作为包一部分的 **\<XX\>** 工具。

 - **\<XX\>_\<YY\>_EXECUTABLE**: 在哪里可以找到 **\<XX\>** 附带的 **\<YY\>** 工具。

 - **\<XX\>_ROOT_DIR**: 在哪里找到 **\<XX\>** 安装的基目录。

 - **\<XX\>_VERSION_\<YY\>**: 如果结果为真，则表示找到了软件包的版本。

 - **\<XX\>_\<YY\>_FOUND**: 如果为假，则 **\<XX\>** 包的可选 **\<YY\>** 部分不可用。

 - **\<XX\>_FOUND**: 如果没有找到或者不想使用 **\<XX\>**，则设置为 **false** 或 **undefined**。

模块在项目中的使用方式如下:

```
find_package(OpenGL)

or

include(${CMAKE_ROOT}/Modules/FindOpenGL.cmake)

or

include(FindOpenGL)
```

## 例子

### Module mode

[Module Mode Demo][1]

```
mkdir -p ~/workspace/.cmake/Install/
mkdir -p ~/workspace/.cmake/Modules/

cmake -DCMAKE_INSTALL_PREFIX=~/workspace/.cmake/Install/ -DCMAKE_MODULE_PATH=~/workspace/.cmake/Modules/ ../packages/gromit/
make
make install

cmake -DCMAKE_INSTALL_PREFIX=~/workspace/.cmake/Install/ -DCMAKE_MODULE_PATH=~/workspace/.cmake/Modules/ ../main
make
```

### Config mode

[Config Mode Demo][2]

```
mkdir -p ~/workspace/.cmake/Install/

cmake -DCMAKE_INSTALL_PREFIX=~/workspace/.cmake/Install/ ../packages/gromit/
make
make install

cmake -DCMAKE_PREFIX_PATH=~/workspace/.cmake/Install/ ../main
make
```

 [1]: https://github.com/P-Fly/build-archives/tree/master/CMake/MasteringCMake/demo/FindingPackages/module-demo
 [2]: https://github.com/P-Fly/build-archives/tree/master/CMake/MasteringCMake/demo/FindingPackages/config-demo
