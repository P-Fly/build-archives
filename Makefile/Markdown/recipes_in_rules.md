# 规则的命令

## 命令的回显

 - 使用 `@` 禁止某个命令的回显。

 - 使用命令行参数 `-n` 或 `--just-print` 显示所有需要执行的命令（但命令不会真正的去执行）。

 - 使用命令行参数 `-s` 或 `--slient` 禁止所有命令的回显。

 - 使用 `.SILENT` 禁止指定目标规则的命令的回显。

## 命令的执行

 - 当 **目标文件** 需要被重建时，此规则定义的命令将会被依次执行。

 - 如果是 **多行命令**，每一行命令将在一个独立的 **子shell** 中被执行。多行相互独立，不存在依赖。

 - **Makefile** 对所有规则命令的解析使用 `SHELL` 所指定的程序。但没有使用系统环境变量中的定义，默认为 `/bin/sh`。

## 命令的并发执行

通过命令行选项 `-j` 或者 `--job`，设置允许多条命令同时被执行。

## 命令的递归执行

在 **Makefile** 中使用 **make** 作为一个命令来执行本身或者其它 **Makefile** 文件的过程。

```
subsystem:
    $(MAKE) -C subdir
```

在递归执行过程中，上层 **Makefile** 可以明确指定将一些变量的定义通过 **环境变量** 的方式传递给子 **Makefile** 过程。

 - 希望将一个变量传递给子 **Makefile** 时，使用 `export` 来声明这个变量。

    ```
    export VARIABLE ...
    ```

 - 不希望将一个变量传递给子 **Makefile** 时，使用 `unexport` 来声明这个变量。

    ```
    unexport VARIABLE ...
    ```

 - 上层传递的变量不会覆盖子 **Makefile** 过程的同名变量定义。

 - 一些特殊的变量会被传递给子 **Makefile**，比如：

    - **CURDIR**：记录当前 **Makefile** 的工作路径。

    - **MAKEFILES**：记录需要读入的 **Makefile** 文件，多个文件之间使用空格分开。

    - **SHELL**：记录执行命令的解释程序的名字。

    - **MAKEFLAGS**：记录 **Makefile** 的命令行选项。

## 定义命令包

使用 `define` 封装一组命令，以后在需要的地方可以通过它的名字进行引用，和C语言中的宏类似。

```
$ cat Makefile
define def_demo
@echo "Expand directive"
@echo "target = $@"
endef

all:
    @$(def_demo)

$ make
Expand directive
target = all
```

需要注意：

 - 命令包的值是作为一个完整的 **Shell** 命令行来处理，即例子中的命令等价于 `@echo "Expand directive"; @echo "target = $@"`。

 - 命令包的展开方式为 **递归展开式的变量**，即在定义时不会被展开，会在命令包被引用的时候被展开。

 - 可以套嵌引用。

 - 可以使用 `override` 声明变量，防止变量的值被命令行中的值替代。

## 空命令

定义一个什么也不做的规则，可以防止 **Makefile** 在执行时试图为这个目标查找隐含命令。

```
target: ;
```
