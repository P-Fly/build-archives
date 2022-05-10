# 内嵌函数

## 调用语法

```
$(FUNCTION ARGUMENTS)

或者

${FUNCTION ARGUMENTS}
```

 - **FUNCTION** 是需要调用的函数名。用户自定义的函数需要通过 **call** 函数来间接调用。

 - **ARGUMENTS** 是函数的参数，参数和函数名之间使用空格进行分割；如果存在多个参数，参数之间使用逗号分开。

 - 参数中存在变量或者函数的引用时，推荐统一使用圆括号，例如：**$(sort $(x))**。

 - 函数处理参数时，参数中如果存在对其它变量或者函数的引用，将首先对这些引用进行展开，展开顺序是按照参数的先后顺序进行的。

 - 当有逗号或者空格作为函数的参数时，需要把它赋给一个变量，在函数的参数中引用这个变量来实现。

    ```
    comma:= ,
    empty:=
    space:= $(empty) $(empty)
    foo:= a b c
    bar:= $(subst $(space),$(comma),$(foo))
    ```

## 文本处理函数

### subset

```
$(subst FROM,TO,TEXT)
```

函数功能：把字符串 **TEXT** 中的 **FROM** 字符替换为 **TO**。

### patsubset

```
$(patsubst PATTERN,REPLACEMENT,TEXT)
```

函数功能：搜索字符串 **TEXT** 中以空格分开的单词，将符合模式 **PTATTERN** 的字符串替换为 **REPLACEMENT**。

```
$(patsubst %.o,%.c,$(objects))
```

等价于：

```
$(objects:.o=.c)
```

### strip

```
$(strip STRINT)
```

函数功能：去掉字符串开头和结尾的空字符，并将其中多个连续空字符合并为一个空字符。

```
STR =     a      b c
LOSTR = $(strip $(STR))

最终结果为 a b c
```

### findstring

```
$(findstring FIND,IN)
```

函数功能：在字符串 **IN** 中查找 **FIND** 字符串。

### filter

```
$(filter PATTERN...,TEXT)
```

函数功能：返回字符串 **TEXT** 中所有符合模式 **PATTERN** 的单词。存在多个模式时，模式表达式之间使用空格分割。

```
sources := foo.c bar.c baz.s ugh.h

foo: $(sources)
    cc $(filter %.c %.s,$(sources)) -o foo
```

### filter-out

```
$(filter-out PATTERN...,TEXT)
```

函数功能：和 **filter** 函数意义相反。

```
objects = main1.o foo.o main2.o bar.o

mains = main1.o main2.o

$(filter-out $(mains),$(objects))
```

### sort

```
 $(sort LIST)
```

函数功能：将字符串以升序排序，并去掉重复的单词。

### word

```
$(word N,TEXT)
```

函数功能：取字符串 **TEXT** 中第 **N** 个单词。

### wordlist

```
$(wordlist S,E,TEXT)
```

函数功能：从字符串 **TEXT** 中取出从 **S** 开始到 **E** 的单词串。

### words

```
$(words TEXT)
```

函数功能：计算字符串 **TEXT** 中单词的数目。

### firstword

```
$(firstword NAMES...)
```

函数功能：取字符串 **NAMES** 中的第一个单词。

## 文件名处理函数

### dir

```
$(dir NAMES...)
```

函数功能：从文件名序列 **NAMES** 中取出各个文件名的目录部分。

### nodir

```
$(notdir NAMES...)
```

函数功能：从文件名序列 **NAMES** 中取出非目录部分。

### suffix

```
$(suffix NAMES...)
```

函数功能：从文件名序列 **NAMES** 中取出各个文件名的后缀。

### basename

```
$(basename NAMES...)
```

函数功能：从文件名序列 **NAMES** 中取出各个文件名的前缀部分。

### addsuffix

```
$(addsuffix SUFFIX,NAMES...)
```

函数功能：为 **NAMES** 中的每一个文件名添加后缀 **SUFFIX**。

### addprefix

```
$(addprefix PREFIX,NAMES...)
```

函数功能：为 **NAMES** 中的每一个文件名添加前缀 **PREFIX**。

### join

```
$(join LIST1,LIST2)
```

函数功能：将字符串 **LIST1** 和字符串 **LIST2** 各单词进行对应连接。

### wildcard

```
$(wildcard PATTERN)
```

函数功能：列出当前目录下所有符合模式 **PATTERN** 的文件名。

```
$(wildcard *.c) 
```

## 调试函数

### error

```
$(error TEXT)
```

函数功能：产生致命错误，并提示 **TEXT** 信息给用户，之后退出 **Makefile** 的执行。

### warning

```
$(warning TEXT)
```

函数功能：不会导致致命错误，而只是提示 **TEXT**。

## 其它函数

### foreach

```
$(foreach VAR,LIST,TEXT)
```

函数功能：将字符串组 **LIST** 中的字符串依次赋给 **VAR**。然后在执行时，如果表达式 **TEXT** 中存在变量 **VAR**，则将其带入表达式 **TEXT** 中参与执行。

 - 该函数首先展开变量 **VAR** 和 **LIST** 的引用。

 - **TEXT** 中的变量或者函数引用会在执行时才被展开。因此如果在 **TEXT** 中存在对 **VAR** 的引用，那么 **VAR** 的值在每一次展开时将会不同。

```
# 注意 find_files 被定义为递归展开式变量，保证了定义时引用不被展开，而是在表达式执行时才被展开。
find_files = $(wildcard $(dir)/*)
dirs := a b c d
files := $(foreach dir,$(dirs),$(find_files))
```

### if

```
$(if CONDITION,THEN-PART[,ELSE-PART])
```

函数功能：如果 **CONDITION** 的展开结果非空，则将 **THEN-PATR** 作为函数的计算表达式，否则将 **ELSE-PART** 作为函数的计算表达式。

```
SUBDIR += $(if $(SRC_DIR),$(SRC_DIR),/home/src)
```

### call

```
$(call VARIABLE,PARAM,PARAM,...)
```

函数功能：对用户自己定义的函数进行引用。

```
reverse = $(2) $(1)
foo = $(call reverse,a,b)
```

 - **VARIABLE** 是变量名，而不是对变量的引用（即不使用 **$** 符号）。

 - 函数对参数的数目没有限制，**$(1)** 为第一个参数，**$(2)** 为第二个参数值。**$(0)** 为 **VARIABLE** 本身。

 - **VARIABLE** 不能定义为直接展开式，只能定义为递归展开式。

 - 函数会保留出现在参数列表中的空字符。建议在变量作为参数值之前，使用 **strip** 函数去掉多余的空格。

### value

**value** 提供了一种在不对变量进行展开的情况下获取其值的方式。但是该函数并不会取消之前已经执行过的替换扩展。

```
foo = foo
bar = $(foo)
baz = $(value bar)

all:
    @echo '$(bar)'
    @echo '$(baz)'

$ make
foo
$(foo)
```

### eval

**eval** 可以构造一个可变的规则结构关系，其中可以使用其它变量和函数。它的参数进行展开后，展开的结果又作为 **Makefile** 的一部分进行语法解析。展开的结果可以包含一个新变量、目标、隐含规则或者是明确规则等。

 - 该函数执行时会对它的参数进行两次展开：

    1. 第一次展开是由函数本身完成的。

    2. 第二次是函数展开后的结果被 **Makefile** 进行解析时展开的。

 - 当第二次进行解析时如果存在引用，那么在函数的参数中应该使用 **$$** 来代替 **$**。

```
# sample Makefile
PROGRAMS = server client

server_OBJS = server.o server_priv.o server_access.o
server_LIBS = priv protocol

client_OBJS = client.o client_api.o client_mem.o
client_LIBS = protocol

# Everything after this is generic
.PHONY: all
all: $(PROGRAMS)

define PROGRAM_template
$(1): $$($(1)_OBJ) $$($(1)_LIBS:%=-l%)
ALL_OBJS += $$($(1)_OBJS)
endef

$(foreach prog,$(PROGRAMS),$(eval $(call PROGRAM_template,$(prog))))

$(PROGRAMS):
    $(LINK.o) $^ $(LDLIBS) -o $@

clean:
    rm -f $(ALL_OBJS) $(PROGRAMS)
```

### origin

获取此变量的定义方式

```
$(origin VARIABLE)
```

函数的返回情况如下：

 - **undefined** 变量没有被定义。

 - **default** 变量是一个内嵌变量。

 - **environment** 变量是一个系统环境变量，并且没有使用命令行选项 **-e**。

 - **environment override** 变量是一个系统环境变量，并且使用了命令行选项 **-e**。

 - **file** 变量在某一个 **Makefile** 文件中被定义。

 - **command line** 变量在命令行中被定义。

 - **override** 变量在 **Makefile** 文件中定义并使用 **override** 指示符声明。

 - **automatic** 变量是自动化变量。

### shell

该函数实现了命令的扩展。使用一个 **shell** 命令作为它的参数，返回此命令的执行结果。

```
START_DATE_TIME := $(shell date +"%Y-%m-%d %T.%N")
```
