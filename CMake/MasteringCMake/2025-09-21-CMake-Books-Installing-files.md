# Installing Files

软件通常安装在与源代码和构建树不同的目录中。**CMake** 提供 **install** 命令来指定项目的安装方式。

**install** 命令的 **Signatures** 如下:

 - **install(TARGETS …)**: 安装与项目内部构建的目标相对应的 **二进制文件**。

 - **install(FILES …)**: 通用文件安装，通常用于软件所需的 **头文件**，**文档** 和 **数据文件**。

 - **install(PROGRAMS …)**: 安装项目未构建的 **可执行文件**，例如 **Shell** 脚本。此参数与 **install(FILES)** 相同，只是安装文件的默认权限包含 **可执行位**。

 - **install(DIRECTORY …)**: 此参数安装整个目录树。它可用于安装包含 **资源(例如图标和图像)的目录**。

 - **install(SCRIPT …)**: 指定用户提供的 **CMake脚本文件**，该文件将在安装过程中执行。通常用于定义规则的 **Pre-install Actions** 或 **Post-install Actions**。

 - **install(CODE …)**: 指定在安装期间执行的用户提供的 **CMake** 代码。这与 **install(SCRIPT)** 类似，但代码在调用中以字符串形式的内联提供。

 - **install(EXPORT …)**: 生成并安装一个 **CMake** 文件，其中包含将目标从安装树导入到另一个项目的代码。

**install** 命令的 **Keyword Arguments** 如下:

 - **DESTINATION**: 此参数指定安装规则将放置文件的位置，后面跟指示该位置的目录路径。

    - 如果目录指定为完整路径，则在安装时会将其评估为 **绝对路径**。

    - 如果目录指定为相对路径，则在安装时会将其评估为 **相对于安装前缀的路径**。

    用户可以通过缓存变量 **CMAKE_INSTALL_PREFIX** 设置前缀。**Linux** 上的默认位置为 **\/usr\/local**

 - **PERMISSIONS**: 此参数对已安装文件设置指定的 **文件权限**。

 - **CONFIGURATIONS**: 此参数指定了安装规则适用的构建配置列表 **(Debug, Release, etc.)**。

 - **COMPONENT**: 此参数指定安装规则适用的安装组件，以方便某些项目将其安装分为多个组件并进行单独打包。

 - **OPTIONAL**:  此参数指定如果要安装的输入文件不存在，则不会报错。如果输入文件存在，则会按请求进行安装。如果不存在，则会静默不安装。

## Installing Targets

**install(TARGETS …)** 的目的是安装在构建过程中创建的一些库和可执行文件。**TARGETS** 后面紧跟着使用 **add_executable** 或者 **add_library** 创建的目标列表。

可以对需要安装的目标文件进行分组，其分类如下:

 - **Executables - RUNTIME**: 由 **add_executable** 创建。

 - **Loadable Modules - LIBRARY**: 由 **add_library** 使用 **MODULE** 选项创建。

 - **Shared Libraries - LIBRARY**: 由 **add_library** 使用 **SHARED** 选项在类 **UNIX** 平台上创建。

 - **Dynamic-link Libraries - RUNTIME**: 由 **add_library** 使用 **SHARED** 选项在 **Windows** 平台上创建。

 - **Import Libraries - ARCHIVE**: 由导出符号的 **Dynamic-link Libraries** 创建的可链接文件。

 - **Static Libraries - ARCHIVE**: 由 **add_library** 使用 **STATIC** 选项创建。

```
install(TARGETS myExecutable DESTINATION bin)
install(TARGETS myStaticLib DESTINATION lib/myproject)
install(TARGETS myPlugin DESTINATION lib)

OR

install(TARGETS mySharedLib
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/myproject)
```

也可以将示例中的项目打包成单独的运行时组件和开发组件，并将每个安装的目标文件分配到相应的组件内。

```
install(TARGETS myExecutable mySharedLib myStaticLib myPlugin
        RUNTIME DESTINATION bin           COMPONENT Runtime
        LIBRARY DESTINATION lib           COMPONENT Runtime
        ARCHIVE DESTINATION lib/myproject COMPONENT Development)
```

## Installing Files

文件的通用安装是使用 **FILES** 签名指定的，后面紧跟着要安装的文件列表。文件将被安装到指定的 **DESTINATION** 目录。

```
install(FILES my-api.h ${CMAKE_CURRENT_BINARY_DIR}/my-config.h
        DESTINATION include)
```

安装文件的默认权限为 **OWNER_WRITE**，**OWNER_READ**，**GROUP_READ** 和 **WORLD_READ**，但可以通过 **PERMISSIONS** 选项指定权限。

```
install(FILES my-rc DESTINATION /etc
        PERMISSIONS OWNER_WRITE OWNER_READ)
```

**RENAME** 可以指定安装文件的名称，仅当安装单个文件时才允许重命名。

```
install(FILES version.h DESTINATION include RENAME my-version.h)
```

## Installing Programs

**PROGRAMS** 后面紧跟着要安装的脚本列表。此命令与 **FILES** 签名相同，只是默认权限还包含 **OWNER_EXECUTE**，**GROUP_EXECUTE** 和 **WORLD_EXECUTE**。

```
install(PROGRAMS my-util.py DESTINATION bin)
```

## Installing Directories

**DIRECTORY** 一般用来安装包含资源文件的完整目录，后面紧跟着要安装的目录列表。

 - 每个输入目录名的最后一个部分会在复制时附加到目地目录下。

    ```
    # data/icons 安装到 share/myproject/icons 目录下
    install(DIRECTORY data/icons DESTINATION share/myproject)
    ```

 - 末尾的斜杠表示最后一个部分为空，并将输入目录的内容安装到目标位置。

    ```
    # data/icons 安装到 share/myproject 目录下
    install(DIRECTORY data/icons/ DESTINATION share/myproject)
    ```

 - 如果没有给出输入目录名称，则将创建目标目录，但不会安装任何内容。

    ```
    install(DIRECTORY DESTINATION share/myproject)
    ```

 - 通过 **DIRECTORY** 安装的文件具有与 **FILES** 相同的默认权限，目录具有与 **PROGRAMS** 相同的默认权限。可以使用 **FILE_PERMISSIONS** 和 **DIRECTORY_PERMISSIONS** 选项修改这些默认权限。

    ```
    install(DIRECTORY data/scripts DESTINATION share/myproject
            FILE_PERMISSIONS
              OWNER_READ OWNER_EXECUTE OWNER_WRITE
              GROUP_READ GROUP_EXECUTE
              WORLD_READ WORLD_EXECUTE
            DIRECTORY_PERMISSIONS
              OWNER_READ OWNER_EXECUTE OWNER_WRITE
              GROUP_READ GROUP_EXECUTE GROUP_WRITE
              WORLD_READ WORLD_EXECUTE
            )
    ```

 - 可以使用 **USE_SOURCE_PERMISSIONS** 选项，在安装过程中使用输入目录中设置的文件权限和目录权限。

    ```
    install(DIRECTORY data/scripts DESTINATION share/myproject
        USE_SOURCE_PERMISSIONS)
    ```

 - 可以使用 **PATTERN ... EXCLUDE** 和 **REGEX ... EXCLUDE** 进行筛选。

    ```
    install(DIRECTORY data/icons DESTINATION share/myproject
            PATTERN ".git" EXCLUDE
            PATTERN "*.txt" EXCLUDE)

    OR

    install(DIRECTORY data/icons DESTINATION share/myproject
            REGEX "/.git$" EXCLUDE
            REGEX "/[^/]*.txt$" EXCLUDE)
    ```

## Installing Scripts

**SCRIPT** 可以使用第三方软件包提供的机制来注册其安装过程，后面紧跟着要执行的 **CMake** 脚本的名称。

## Installing Code

**CODE** 可以将脚本代码内联放置在对应的 **install** 命令中。
