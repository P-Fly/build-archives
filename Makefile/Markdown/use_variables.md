# 变量

在 **Makefile** 中，变量是一个名字，代表一个文本字符串。

## 变量的引用

 - 多字符变量名的引用只能是 `$(VARIABLE_NAME)` 或者 `${VARIABLE_NAME}` 的格式。

 - 在规则命令行中的 **Shell 变量**，可以使用 `$VARIABLE_NAME` 的格式。

 - 在命令或者文件名中使用 `$` 时，需要用 `$$` 来表示。

    ```
    SUBDIRS := foo bar baz

    .PHONY : subdir

    subdir :
        @for dir in $(SUBDIRS); do \
            $(MAKE) –C $$dir       \
        done
    ```

## 变量的赋值

 1. 递归展开式变量：

    - 通过 `=` 或者 `define` 定义的变量。

    - 在变量 **定义** 时，它所引用的其它变量不会被立即展开（变量被定义后是一个包含其它变量的引用的文本串）。

    - 在变量 **引用** 时，它所引用的其它变量才会被替换展开。

    ```
    $ cat Makefile
    foo = null
    bar = $(foo)
    foo = foo

    all:
        @echo $(bar)

    $ make
    foo
    ```

    - 优点：

        - 在变量定义时，可以引用之前没有定义的其它变量。

    - 缺点：

        - 容易出现变量的递归定义，而导致变量展开错误。

        ```
        # 错误的用法
        CFLAGS = $(CFLAGS) –O
        ```

        - 如果在变量定义时使用了函数，那么函数会在该变量每次引用的地方都重新执行一次。

 2. 直接展开式变量

    - 通过 `:=` 定义的变量。

    - 在变量 **定义** 时，它所引用的其它变量会被立即展开（变量被定义后就是一个实际的文本串，不再包含任何变量的引用）。

    ```
    $ cat Makefile
    foo := null
    bar := $(foo)
    foo := foo

    all:
        @echo $(bar)

    $ make
    null
    ```

 3. 条件赋值的变量

    - 通过 `?=` 进行赋值的变量。

    - 只有此变量在之前没有赋值的情况下，才会被赋值。等价于：

    ```
    ifeq ($(origin VARIABLE_NAME), undefined)
    VARIABLE_NAME = bar
    endif
    ```

 4. 追加变量

    - 在 **Makefile** 中使用 `+=` 来实现对一个变量值的追加操作。

    - 如果变量没有定义，`+=` 会自动变成 `=` 的风格。

    - 如果变量已经定义，`+=` 就继承之前的变量风格。

    - 直接展开式的追加过程：

    ```
    variable := value
    variable += more

    # 等价于

    variable := value
    variable := $(variable) more
    ```

    - 递归展开式的追加过程：

    ```
    variable = value
    variable += more

    # 等价于

    temp = value
    variable = $(temp) more
    ```

## 变量的高级用法

 1. 变量的替换引用

该方法将其值中的后缀字符串用指定的字符串替换。比如 `bar := $(foo:.o=.c)` 或者 `bar := $(foo:%.o=%.c)`。

 2. 变量的套嵌引用

当一个变量名之中包含对其它变量的引用时，这种情况称为 **变量的套嵌引用**。

```
x = y
y = z
a := $($(x))
```

## override 指示符

该指示符可以改变或者追加那些使用命令行指定的变量的定义。

```
$ cat Makefile
override CFLAGS := -g

all:
    @echo CFLAGS = $(CFLAGS)

$ make CFLAGS=-O2
CFLAGS = -g
```

## 目标/模式指定变量

允许对于相同的变量名，根据 **目标** 指定不同的值，该值只在指定它的目标的上下文中有效，有点类似于 **局部变量**。

 1. 目标指定变量

    ```
    TARGET ... : VARIABLE-ASSIGNMENT

    或者

    TARGET ... : override VARIABLE-ASSIGNMENT
    ```

    - 可以使用任何一个有效的赋值方式：`=`、`:=`、`+=`、`?=`。

    - **目标指定变量** 和 **同名的全局变量** 属于两个不同的变量，赋值方式和值相互不会影响。

    - 为了防止 **目标指定变量** 被命令行或环境变量覆盖，可以使用指示符 `override` 对其进行声明。

    - 该变量会作用到由这个目标所引发的所有的规则中去（包括**多规则目标**）。

    ```
    $ cat Makefile
    CFLAGS := -Wall

    all: foo bar
        @echo $@: CFLAGS = $(CFLAGS)

    foo: CFLAGS += -O2
    bar: CFLAGS += -g

    foo:
        @echo $@: CFLAGS = $(CFLAGS)

    bar:
        @echo $@: CFLAGS = $(CFLAGS)

    $ make all
    foo: CFLAGS = -Wall -O2
    bar: CFLAGS = -Wall -g
    all: CFLAGS = -Wall
    ```

 2. 模式指定变量

    将一个变量值指定到所有符合特定模式的目标上去。

    ```
    PATTERN ... : VARIABLE-ASSIGNMENT

    或者

    PATTERN ... : override VARIABLE-ASSIGNMENT
    ```

    - 对于同一个变量如果使用 **追加方式**，通常目标的局部变量值的顺序是：**为所有规则定义的全局值 + 引发它所在规则被执行的目标所指定值 + 它所符合的模式指定值 + 此目标所指定的值**。

    ```
    %.o : CFLAGS += -O
    ```

## 特殊变量

### .VARIABLES

它被展开以后是此引用点之前，**Makefile** 中所定义的所有全局变量列表。

### MAKEFILES

如果定义了一个 **MAKEFILES** 环境变量，**Makefile** 首先将此变量的值作为需要读入的文件，多个文件之间使用空格分开。

```
$ cat submake
foo:
    @echo foo

$ cat Makefile
target: foo

$ export MAKEFILES='submake'
$ make
foo
```

它和 **include** 的区别如下：

 1. **MAKEFILES** 所定义的文件中的 **目标**，不会被作为 **终极目标**。

 2. **MAKEFILES** 所定义的文件是否存在，不会导致 **Makefile** 的错误。

 3. **Makefile** 在执行时，首先读取的是 **MAKEFILES** 所指定的文件列表，之后才是工作目录下的其它文件。

注意：实际应用中很少设置 **MAKEFILES**。因为此变量类似于 **全局变量**。一旦设置了此变量，在多级 **Makefile** 调用时，每一级都会读取 **MAKEFILES** 所指定的文件，容易导致执行出现混乱。

### MAKEFILE_LIST

当 **Makefile** 解析指示符指定的子 **Makefile** 文件之前，会将读取的文件名依次追加到变量 **MAKEFILE_LIST** 的定义域中。

我们可以利用该特性，在每个 **Makefile** 文件头获取当前路径和文件名：

```
THIS_DIR := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))
THIS_FILE := $(lastword $(MAKEFILE_LIST))
```
