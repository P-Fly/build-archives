# ifdef多条件判断

在使用 **Makefile** 时，有时需要实现如下的逻辑:

```
ifdef VAR1 || VAR2 || VAR3

or

ifdef VAR1 && VAR2 && VAR3
```

但 **ifdef** 只支持单一的变量操作，在网上搜索了一下，有一个非常好的解决方案。

[原文链接][1]

# ||操作

```
ifdef_any_of = $(filter-out undefined,$(foreach v,$(1),$(origin $(v))))

ifneq ($(call ifdef_any_of,VAR1 VAR2),)
result=define
else
result=undefine
endif

all:
    @echo $(result)
```

# &&操作

```
ifndef_any_of = $(filter undefined,$(foreach v,$(1),$(origin $(v))))

ifeq ($(call ifndef_any_of,VAR1 VAR2),)
result=define
else
result=undefine
endif

all:
    @echo $(result)
```

[1]: https://stackoverflow.com/questions/5584872/complex-conditions-check-in-makefile
