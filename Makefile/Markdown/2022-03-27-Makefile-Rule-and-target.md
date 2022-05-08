# 规则和目标

## 规则

**Makefile** 中，**规则** 描述了何种情况下使用什么命令来重建一个特定的文件，一个简单的 **Makefile** 规则描述如下：

```
TARGET... : PREREQUISITES...
    COMMAND
    ...
```

 - **target**： 规则的目标。通常是最后需要生成的文件名，或者为了实现这个目的而必需生成的中间过程文件名，也可以是一个伪目标。

 - **prerequisites**： 规则的依赖。是生成规则目标所需要的文件名列表，文件名也可以是其它的规则目标。

 - **command**： 规则的命令行。是规则所要执行的动作集合，是在任何一个目标的依赖文件发生变化后重建目标的动作描述。

一句话解释：**规则包含了目标和依赖的关系，以及更新目标所要求的命令。**

**规则的书写建议是：单目标，多依赖。尽量做到一个规则中只存在一个目标文件，但可以有多个依赖文件。**

## 依赖

```
TARGETS: NORMAL-PREREQUISITES | ORDER-ONLY-PREREQUISITES
```

 1. **常规依赖**：如果 **规则** 中的任何一个 **依赖文件** 被更新，则 **目标文件** 相应地也应该被更新。

 2. **Order-Only依赖**：只有第一次构造 **目标文件** 时才会使用 **ORDER-ONLY-PREREQUISITES**，后面即使 **ORDER-ONLY-PREREQUISITES** 发生改变，也不会重新构造。只有 **NORMAL-PREREQUISITES** 中的文件发生改变时才重新构造 **目标文件**。

    ```
    OBJDIR := out
    OBJS := $(addprefix $(OBJDIR)/, foo.o bar.o)

    $(OBJDIR)/%.o : %.c
        $(CC) -c $< -o $@

    all: $(OBJS)

    $(OBJS): | $(OBJDIR)

    $(OBJDIR):
        mkdir $(OBJDIR)
    ```

    - **\$(OBJS): \| \$(OBJDIR)** 为 **Order-Only依赖**，表明如果 **$(OBJDIR)** 目录不存在，则在 **.o** 文件生成前创建该目录。

    - 如果改为 **\$(OBJS): \$(OBJDIR)**，因为最后一个 **.o** 文件会修改 **$(OBJDIR)** 目录的时间，则后续编译时，除最后一个 **.o** 文件外，其它的依赖文件都会被重新编译。

## 终极目标

**终极目标** 就是 **Makefile** 最终所要重建的某个规则的目标。默认情况下，第一个规则中的第一个目标就是 **终极目标**。因此 **终极目标** 的编译规则往往会描述整个工程或者程序的编译过程和规则。

 1. 如果 **Makefile** 中的第一个规则有多个目标，那么多个目标中的第一个目标就是默认的 **终极目标**。

 2. 可以通过命令行直接指定本次执行过程的 **终极目标**，如 **make clean**。此时 **Makefile** 设置一个特殊变量 **MAKECMDGOALS** 来记录命令行参数指定的 **终极目标**。没有通过命令行参数指定 **终极目标** 时，此变量为空。

    ```
    foo:
        @echo "MAKECMDGOALS = ${MAKECMDGOALS}"

    $ make
    MAKECMDGOALS =

    $ make foo
    MAKECMDGOALS = foo
    ```

 3. 可以指定一个在 **Makefile** 中不显示存在的目标作为 **终极目标**，前提是存在一个对应的 **隐含规则** 能够生成该目标。

## 伪目标

**伪目标** 不代表一个真正的文件名，在执行 **Makefile** 时可以指定这个目标来执行其所在规则定义的命令。将一个目标声明为 **伪目标** 需要将它作为特殊目标 **.PHONY** 的依赖。

```
.PHONY: clean
clean:
    rm *.o
```

使用 **伪目标** 有两点原因：

 1. 避免在规则中定义的目标和目录下的文件出现名字冲突。

 2. 提高执行 **Makefile** 的效率。

    - 使用 **并行方式** 执行 **Makefile**。

    ```
    SUBDIRS = foo bar baz

    .PHONY: subdirs $(SUBDIRS)

    subdirs: $(SUBDIRS)

    $(SUBDIRS):
        $(MAKE) -C $@

    foo: baz baz
    ```

    - 对于 **伪目标**，**Makefile** 在执行此规则时不会试图去查找隐含规则来创建这个目标。

## 强制目标

**强制目标** 是指一个规则没有命令或者依赖，并且它的目标不是一个存在的文件名。在执行此规则时，目标总会被认为是 **已经被更新过**。

```
target: FORCE
    touch target

FORCE: ;
```

 - 目标 **FORCE** 为一个 **强制目标**，并作为 **目标文件** 的依赖。

 - 在执行 **make target** 时，由于 **FORCE** 被认为总是需要更新，所以 **目标文件** 所在的规则总会被执行。

## 空目标文件

**空目标文件** 是指 **目标文件** 存在，但我们对该文件的具体内容并不关心。只是利用该文件的时间戳，记录此规则命令的 **最后执行时间**。通常此文件是一个空文件。

```
print: foo.txt
    cat $?
    touch print
```

我们对目标文件 **print** 的内容并不关心，只是用来记录上一次执行此规则命令的时间。如果依赖文件 **foo.txt** 有更新，此目标所在规则的命令行将被执行。

## 多目标规则

**多目标** 是指一个规则中可以有多个目标，规则所定义的命令对所有的目标有效。

 1. **多目标** 的规则相当于多个 **独立的规则**，只是这些规则具有相同的 **依赖文件**。

 2. **多目标** 规则中，可以根据自动化变量 **$@** 对不同的目标使用不同的命令参数。

 3. **多目标** 的规则并不能自动改变依赖文件。要实现这个目的，需要用到 **静态模式**。

```
object := foo baz
input := foo.in baz.in

edit: $(object)
    touch $@

$(object): $(input) def.h
    touch $@
```

## 多规则目标

**多规则目标** 是指一个目标作为多个规则的目标出现。

 1. **多规则目标** 的 **所有依赖** 将会被合并成一个 **依赖列表**，任何一个 **依赖** 更新时，将会重建该 **目标**。

 2. 重建 **多规则目标** 的命令只能出现在一个规则中
。如果 **多个规则** 同时给出重建此 **多规则目标** 的命令，**Makefile** 将使用 **最后一个规则** 所定义的命令，同时提示警告信息。

 3. 如果需要对 **相同的目标** 依据 **不同的依赖**，使用 **不同的命令**，可以使用 **双冒号规则**。

 4. 如果所有的 **多规则目标** 都没有定义命令，**Makefile** 会寻找一个合适的 **隐含规则** 来重建此 **多规则目标**。

```
target: foo
    @echo foo
target: baz
    @echo bar

$ make
Makefile:5: warning: overriding recipe for target 'target'
Makefile:2: warning: ignoring old recipe for target 'target'
bar
```

## 双冒号规则

**双冒号规则** 就是使用 **::** 代替 **:** 得到的规则。

注意：**Makefile** 中，一个目标可以出现在多个规则中，但是必须是同一类规则（要么都是普通规则，要么都是双冒号规则）。

 1. 当 **依赖** 比 **目标** 更新时，**规则** 将会被执行。

 2. 当引用没有 **依赖** 而只有 **命令** 的 **双冒号规则** 时，无论 **规则的目标文件** 是否存在，该 **命令** 都会被无条件执行。

 3. 当同一个 **目标** 在多个 **双冒号规则** 中时，这些不同的规则会被独立的处理，不会被合并。即：每一个 **依赖** 被改变之后，**Makefile** 只执行 **此规则定义的命令**。

```
release:: prj1
    @touch release
    @echo "release prj1"
release:: prj2
    @touch release
    @echo "release prj2"
clean:
    @rm -rf release prj1 prj2

$ touch prj1 prj2
$ make release
release prj1
release prj2

$ echo 1 > prj1
$ make release
release prj1

$ echo 2 > prj2
$ make release
release prj2
```

## 静态模式

**静态模式** 是这样一个规则：规则存在多个 **目标文件**，并且可以根据 **目标文件** 的名字来自动构造出 **依赖文件**。并且这些 **依赖文件** 是相类似的。

```
TARGETS ...: TARGET-PATTERN: PREREQ-PATTERNS
    COMMANDS
    ...
```

**TAGET-PATTERN** 和 **PREREQ-PATTERNS** 说明了如何为 **目标文件** 生成 **依赖文件**。

```
objects = foo.o bar.o
all: $(objects)

$(objects): %.o: %.c
    $(CC) -c $(CFLAGS) $< -o $@
```

## 自动产生依赖

工程中可能需要书写一些规则来描述 **目标文件** 和 **头文件** 的依赖关系。**GCC** 支持使用 **-M** 和 **-MM** 的选项来实现此功能 (通过查找源文件中的 **#include** 来实现)。

```
$ cat main.c
#include <stdio.h>
#include "defs.h"

# 输出的依赖关系中包含标准库头文件
$ gcc -M main.c
main.o: main.c /usr/include/stdc-predef.h /usr/include/stdio.h \
 /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
 /usr/include/features.h /usr/include/x86_64-linux-gnu/sys/cdefs.h \
 /usr/include/x86_64-linux-gnu/bits/wordsize.h \
 /usr/include/x86_64-linux-gnu/bits/long-double.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs.h \
 /usr/include/x86_64-linux-gnu/gnu/stubs-64.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stddef.h \
 /usr/lib/gcc/x86_64-linux-gnu/9/include/stdarg.h \
 /usr/include/x86_64-linux-gnu/bits/types.h \
 /usr/include/x86_64-linux-gnu/bits/timesize.h \
 /usr/include/x86_64-linux-gnu/bits/typesizes.h \
 /usr/include/x86_64-linux-gnu/bits/time64.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__mbstate_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__fpos64_t.h \
 /usr/include/x86_64-linux-gnu/bits/types/__FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/FILE.h \
 /usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h \
 /usr/include/x86_64-linux-gnu/bits/stdio_lim.h \
 /usr/include/x86_64-linux-gnu/bits/sys_errlist.h defs.h

# 输出的依赖关系中不包含标准库头文件
$ gcc -MM main.c
main.o: main.c defs.h
```
