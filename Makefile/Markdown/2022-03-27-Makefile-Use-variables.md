# 使用变量

在 **Makefile** 中，变量是一个名字，代表一个 **文本字符串**。

## 变量的引用

 - 多字符变量名的引用只能是 **$(VARIABLE_NAME)** 或者 **${VARIABLE_NAME}** 的格式。

 - 在规则命令行中的 **Shell变量**，可以使用 **$VARIABLE_NAME** 的格式。

 - 在命令或者文件名中使用 **$** 时，需要用 **$$** 来表示。

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

    - 通过 **=** 或者 **define** 定义的变量。

    - 在变量 **定义** 时，它所引用的其它变量不会被立即展开，变量是一个包含其它变量的引用的文本串。

    - 在变量 **引用** 时，它所引用的其它变量才会被替换展开。

    ```
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

        - 容易出现变量的 **递归定义**，而导致变量展开错误。

        ```
        # 错误的用法
        CFLAGS = $(CFLAGS) –O
        ```

        - 如果在变量定义时使用了函数，那么函数会在该变量每次引用的地方都重新执行一次。

 2. 直接展开式变量

    - 通过 **:=** 定义的变量。

    - 在变量 **定义** 时，它所引用的其它变量会被立即展开，变量被定义后就是一个实际的文本串，不再包含任何变量的引用。

    ```
    foo := null
    bar := $(foo)
    foo := foo

    all:
        @echo $(bar)

    $ make
    null
    ```

    另外，可以使用直接展开式变量来定义空格：

    ```
    empty   :=
    space   := $(empty) $(empty)
    ```

 3. 条件赋值的变量

    - 通过 **?=** 进行赋值的变量。

    - 只有此变量在之前没有赋值的情况下，才会被赋值。等价于：

    ```
    ifeq ($(origin VARIABLE_NAME), undefined)
    VARIABLE_NAME = bar
    endif
    ```

 4. 追加变量

    - 在 **Makefile** 中使用 **+=** 来实现对一个变量值的追加操作。

    - 如果变量没有定义，**+=** 会自动变成 **=** 的风格。

    - 如果变量已经定义，**+=** 就继承之前的变量风格。

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

    该方法将其值中的 **后缀字符串** 用指定的字符串替换。

    ```
    foo := a.o b.o c.o
    bar := $(foo:.o=.c)

    或者

    foo := a.o b.o c.o
    bar := $(foo:%.o=%.c)
    ```

 2. 变量的套嵌引用

    当一个变量名之中包含对其它变量的引用时，这种情况称为 **变量的套嵌引用**。

    ```
    a_dirs := dir_a1 dir_a2
    b_dirs := dir_b1 dir_b2

    a_files := file_a1 file_a2
    b_files := file_b1 file_b2

    ifeq "$(use_a)" "yes"
        use := a
    else
        use := b
    endif

    ifeq "$(use_dirs)" "yes"
        type := dirs
    else
        type := files
    endif

    target := $($(use)_$(type))
    ```

    **使用限制：** 不能通过指定需要调用的函数名称来实现对这个函数的调用，因为套嵌引用在展开之前已经完成了对函数名的识别测试。

    ```
    ifdef do_sort
    func := sort
    else
    func := strip
    endif

    bar := a d b g q c
    foo := $($(func) $(bar))

    此时 foo 最终赋值为字符串："sort a d b g q c" 或 "strip a d g q c"
    ```

## override 指示符

通过使用指示符 **override** 来声明不希望该变量被命令行中的值替代。

```
override foo := foo

all:
    @echo foo = $(foo)

$ make foo=bar
foo = foo
```

需要注意：

 - 变量在定义时使用了 **override**，则后续对它的值进行追加时，也需要使用 **override** 指示符。否则对此变量值的追加不会生效。

    ```
    override CFLAGS := -Wall –g

    CFLAGS += -DDebug

    all:
        @echo CFLAGS = $(CFLAGS)

    $ make
    CFLAGS = -Wall –g
    ```

 - 可以通过 **override** 指示符来指定一些通用的参数，而在命令行中指定一些特殊的参数。

    ```
    override CFLAGS += -Wall –g

    all:
        @echo CFLAGS = $(CFLAGS)

    $ make CFLAGS=Debug
    CFLAGS = Debug -Wall –g
    ```

## 多行定义

使用 **define**指示符，可以定义一个包含多行字符串的 **命令包**。类似于其它语言的函数封装，以后在需要用到的地方可以通过它的名字来对其进行引用。

```
define run-yacc
    yacc $(firstword $^)
    mv y.tab.c $@
endef

foo.c : foo.y
    $(run-yacc)
```

 - 命令包的值是作为一个完整的 **Shell** 命令行来处理。

 - 命令包的展开方式为 **递归展开式的变量**，即在定义时不会被展开，会在命令包被引用的时候被展开。

 - 可以套嵌引用。

 - 可以使用 **override** 声明变量，防止变量的值被命令行中的值替代。

## 系统环境变量

在 **Makefile** 中，可以引用任何已定义的 **系统环境变量**：

 - 在 **Makefile** 中可以覆盖同名的 **系统环境变量**，但只在当前的执行过程中有效。当使用 **-e** 参数时，**Makefile** 和命令行中定义的变量不会覆盖同名的 **系统环境变量**，相当于为每个 **系统环境变量** 增加了 **override** 指示符。

 - **Makefile** 的递归调用中，所有的 **系统环境变量** 会被传递给下一级。如果普通变量需要传递给 **子Makefile** 时，需要使用 **export** 指示符来进行声明。

 - **SHELL** 是一个特殊的 **系统环境变量**，它用来指定用户和系统的交互接口。而 **Makefile** 是使用 **/bin/sh** 作为其命令行解释程序，因此 **Makefile** 在执行之前会将 **SHELL** 设置为 **/bin/sh**。

## 目标/模式指定变量

允许对于相同的变量名，根据 **目标** 指定不同的值，该值只在指定它的目标的上下文中有效，有点类似于 **局部变量**。

 1. 目标指定变量

    ```
    TARGET ... : VARIABLE-ASSIGNMENT

    或者

    TARGET ... : override VARIABLE-ASSIGNMENT
    ```

    - 可以使用任何一个有效的赋值方式：**=**、**:=**、**+=**、**?=**。

    - **目标指定变量** 和 **同名的全局变量** 属于两个不同的变量，赋值方式和值相互不会影响。

    - 为了防止 **目标指定变量** 被命令行或环境变量覆盖，可以使用指示符 **override** 对其进行声明。

    - 该变量会作用到由这个目标所引发的所有的规则中去（包括 **多规则目标**）。

    ```
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

## 自动化变量

**自动化变量** 的取值是根据所执行规则的 **目标文件** 和 **依赖文件** 的名称来决定的：

 - **\$\@**：表示规则的目标文件名。

 - **\$\%**：当规则的目标文件是一个静态库文件时，代表静态库的一个成员名。如果目标不是静态库文件，其值为空。

 - **\$\<**：规则的第一个依赖文件名。如果是一个目标文件使用隐含规则来重建，则它代表由隐含规则加入的第一个依赖文件。

 - **\$\?**：所有比目标文件更新的依赖文件列表，空格分割。如果目标是静态库文件名，代表的是库成员。

 - **\$\^**：规则的所有依赖文件列表，使用空格分隔。如果目标是静态库文件，它所代表的只能是所有库成员名。**\$\^** 会去掉重复的依赖文件。

 - **\$\+**：类似 **\$\^**，但是它保留了依赖文件中重复出现的文件。

 - **\$\***：在模式规则和静态模式规则中，代表 **茎**。**茎** 是目标模式中 **%** 所代表的部分。

## 隐含变量

在内嵌 **隐含规则** 的命令中，所使用的预定义变量被称为 **隐含变量**。

这些变量可以被重定义：在 **Makefile** 中显示定义，通过命令行参数进行定义，或者在系统环境变量中被定义。

也可以使用 **-R** 选项来取消所有的隐含变量，这同时也将取消所有的隐含规则。

**隐含规则** 中所使用的 **隐含变量** 如下：

 - 代表命令的变量：

    - **AR**：函数库打包程序，可创建静态库 **.a**。默认是 **ar**。

    - **AS**：汇编程序。默认是 **as**。

    - **CC**：**C** 编译程序。默认是 **cc**。

    - **CXX**：**C++** 编译程序。默认是 **g++**。

    - **CO**：从 **RCS** 中提取文件的程序。默认是 **co**。

    - **CPP**：**C预处理器**（输出是标准输出设备）。默认是 **$(CC) -E**。

    - **FC**：编译器和预处理 **Fortran** 和 **Ratfor** 源文件的编译器。默认是 **f77**。

    - **GET**：从 **SCCS** 中提取文件程序。默认是 **get**。

    - **LEX**：将 **Lex** 语言转变为 **C** 或 **Ratfo** 的程序。默认是 **lex**。

    - **PC**：**Pascal** 语言编译器。默认是 **pc**。

    - **YACC**：**Yacc** 文法分析器。默认命令是 **yacc**。

    - **YACCR**：**Yacc** 文法分析器（针对于 **Ratfor** 程序）。默认是 **yacc -r**。

    - **MAKEINFO**：转换 **Texinfo** 源文件到 **Info** 文件程序。默认是 **makeinfo**。

    - **TEX**：从 **TeX** 源文件创建 **TeX DVI** 文件的程序。默认是 **tex**。

    - **TEXI2DVI**：从 **Texinfo** 源文件创建 **TeX DVI** 文件的程序。默认是 **texi2dvi**。

    - **WEAVE**：转换 **Web** 到 **TeX** 的程序。默认是 **weave**。

    - **CWEAVE**：转换 **C Web** 到 **TeX** 的程序。默认是 **cweave**。

    - **TANGLE**：转换 **Web** 到 **Pascal** 语言的程序。默认是 **tangle**。

    - **CTANGLE**：转换 **C Web** 到 **C**。默认是 **ctangle**。

    - **RM**：删除命令。默认是 **rm -f**。

 - 代表命令参数的变量：

    - **ARFLAGS**：执行 **AR** 命令的命令行参数。默认是 **rv**。

    - **ASFLAGS**：执行汇编语器 **AS** 的命令行参数。

    - **CFLAGS**：执行 **CC** 编译器的命令行参数。

    - **CXXFLAGS**：执行 **g++** 编译器的命令行参数。

    - **COFLAGS**：执行 **co** 的命令行参数。

    - **CPPFLAGS**：执行 **C预处理器** 的命令行参数。默认是 **cc -E**。

    - **FFLAGS**：**Fortran** 语言编译器 **f77** 执行的命令行参数。

    - **LDFLAGS**：链接器参数。

    - **LFLAGS**：**Lex** 文法分析器参数。

    - **PFLAGS**：**Pascal** 语言编译器参数。

    - **RFLAGS**：**Ratfor** 程序的 **Fortran** 编译器参数。

    - **YFLAGS**：**Yacc** 文法分析器参数。

## 特殊变量

### .VARIABLES

该变量被展开以后是此引用点之前，**Makefile** 中所定义的所有全局变量列表。

### MAKELEVEL

该变量被用来跟踪 **Makefile** 递归调用过程的调用深度。

### MAKEFILES

如果定义了一个 **MAKEFILES** 环境变量，**Makefile** 首先将此变量的值作为需要读入的文件，多个文件之间使用空格分开。

它和 **include** 的区别如下：

 1. **MAKEFILES** 所定义的文件中的 **目标**，不会被作为 **终极目标**。

 2. **MAKEFILES** 所定义的文件是否存在，不会导致 **Makefile** 的错误。

 3. **Makefile** 在执行时，首先读取的是 **MAKEFILES** 所指定的文件列表，之后才是工作目录下的其它文件。

注意：实际应用中很少设置 **MAKEFILES**。因为一旦设置了此变量，在多级 **Makefile** 调用时，每一级都会读取 **MAKEFILES** 所指定的文件，容易导致执行混乱。

### MAKEFILE_LIST

当 **Makefile** 解析指示符指定的 **子Makefile** 文件之前，会将读取的文件名依次追加到变量 **MAKEFILE_LIST** 的定义域中。

我们可以利用该特性，在每个 **Makefile** 文件头获取当前路径和文件名：

```
THIS_DIR := $(shell dirname $(abspath $(lastword $(MAKEFILE_LIST))))
THIS_FILE := $(lastword $(MAKEFILE_LIST))
```
