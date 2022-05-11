# 隐含规则

**隐含规则** 为 **Makefile** 提供了重建 **目标文件** 的通用方法，并且不需要明确地给出重建的细节描述。

## 隐含规则的使用

 - **隐含规则** 提供了 **目标文件** 的基本 **依赖关系**，以及重建目标需要使用的 **命令行**。

 - 每一个 **隐含规则** 都存在一个 **目标模式** 和 **依赖模式** 关系，而且一个 **目标模式** 可以对应多个 **依赖模式**。

 - **依赖文件** 在没有一个规则明确描述它的情况下，会将其作为一个目标尝试执行 **隐含规则**。

 - **隐含规则** 只提供最基本的 **依赖文件**，当需要增加 **依赖文件** 时，需要使用 **没有命令行的规则** 来明确说明。

 - **Makefile** 会尝试使用 **隐含规则** 来执行 **没有命令行的规则** 和 **双冒号规则**，如果不想让一个没有命令行的规则执行 **隐含规则**，可以使用 **空命令** 来实现。

## 常见隐含规则

 - C：**N.o** 自动由 **N.c** 生成，执行命令为： **$(CC) -c $(CPPFLAGS) $(CFLAGS)**。

 - C++：**N.o** 自动由 **N.cc** 或者 **N.C** 生成，执行命令为：**$(CXX) -c $(CPPFLAGS) $(CFLAGS)**。

 - Pascal：**N.o** 自动由 **N.p** 创建，执行命令为：**$(PC) -c $(PFLAGS)**。

 - 汇编和需要预处理的汇编程序：

    - **N.s** 是不需要预处理的汇编源文件，**N.S** 是需要预处理的汇编源文件，汇编器为 as 。

    - **N.o** 可自动由 **N.s** 生成，执行命令为：**$(AS) $(ASFLAGS)**。

    - **N.s** 可自动由 **N.S** 生成，执行命令为：**$(CPP) $(CPPFLAGS)**。

 - 链接单一的object文件：**N** 可自动由 **N.o** 生成，通过C编译器使用链接器，执行命令为：**$(CC) $(LDFLAGS) N.o $(LOADLIBES) $(LDLIBS)**。此规则仅适用于由一个源文件直接产生可执行文件的情况。

## 隐含规则链

如果一个 **目标文件** 需要一系列 **隐含规则** 才能完成它的创建，这个系列称为 **隐含规则链**。

## 模式规则

**模式规则** 类似于普通规则。只是在 **模式规则** 中，目标名需要包含模式字符 **%**。

```
%.o : %.c
    $(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
```

 - 同样一个 **模式规则** 可以存在多个目标。**多目标的模式规则** 和 **普通多目标规则** 有些不同：

    - **普通多目标规则** 对应多个独立的规则。

    - **多目标的模式规则** 被作为一个整体来处理。所有 **目标文件** 共同拥有 **依赖文件** 和 **规则的命令行**。

    - 当 **目标文件** 符合 **模式规则** 时，规则定义的命令将会被执行，同时规则不会再去检查其它 **同名但后缀不同的目标文件** 是否符合 **模式规则**。

    ```
    foo := a.foo
    bar := a.bar

    all: $(foo) $(bar)

    %.foo %.bar :
        @echo $@

    $ make
    a.foo   # 执行了 a.foo, 不会执行 a.bar
    ```

 - 当一个 **目标文件** 同时符合多个 **模式规则** 时，第一个匹配的 **模式规则** 会作为重建规则。

 - **明确规则** 优先于 **隐含规则**。

 - 依赖文件存在或者被提及的规则，优先于那些需要使用 **隐含规则** 来创建其依赖文件的规则。

### 模式的匹配

 - **模式规则** 中，目标模式由 **前缀、后缀、模式字符 **%** 组成，这三个部分允许两个同时为空。

 - 进行目标文件匹配时，只进行基本文件名的匹配。匹配成功后，再将目录部分加入到匹配之后的文件名前。**（即目录不参与模式匹配）**。

### 重建内嵌隐含规则

 - 我们可以对 **内嵌隐含规则** 进行重建。重建时，需要使用相同的 **目标模式** 和 **依赖模式**，命令可以重新指定。

    ```
    %.o : %.c
        $(CC) $(CFLAGS) –D__DEBUG__   $< -o $@
    ```

 - 也可以使用一个没有命令行的规则取消 **内嵌隐含规则**。

    ```
    %.o : %.s
    ```

## 缺省规则

当需要重建的目标文件没有可用的命令时，就执行 **缺省规则** 的命令。

 - 可以使用 **最终万用规则**。

    ```
    %::
        ...
    ```

 - 使用 **.DEFAULT** 关键字。

    ```
    .DEFAULT :
        ...
    ```

## 后缀规则

**后缀规则** 是一种古老定义 **隐含规则** 的方式，现在已经被 **模式规则** 所替代。

 1. 双后缀规则：定义了目标文件和依赖文件的后缀。

    ```
    .c.o:
        $(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

    # 等价于模式规则

    %o : %c
        $(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
    ```

 2. 单后缀规则：定义了源文件名的后缀。

    ```
    .c:
        ...

    # 等价于模式规则

    % : %.c
        ...
    ```

需要注意：

 - **后缀规则** 中不存在任何 **依赖文件**。否则，此规则将被当做 **普通规则**。

 - 没有命令行的 **后缀规则** 是没有任何意义的。

 - 可以增加或删除可被识别的后缀：

    - 通过特殊目标 **SUFFIXES** 增加或重定义可被识别的后缀。

    ```
    .SUFFIXES:          # 删除所有可被识别的后缀
    .SUFFIXES: .c .o .h # 增加可被识别的后缀
    ```

    - 使用命令行参数 **-r** 或 **-no-builtin-rules** 清空已定义的可识别后缀。