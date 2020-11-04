# 条件执行

条件语句可以根据一个变量的值来控制执行或者忽略 **Makefile** 的特定部分。

## 语法格式

```
CONDITIONAL-DIRECTIVE
TEXT-IF-TRUE
else
TEXT-IF-FALSE
endif
```

## 关键字

 - `ifeq`：用来判断参数是否相等。

 - `ifneq`：用来判断参数是否不相等。

 - `ifdef`：用来判断一个变量是否定义。

    - 如果变量的值非空，那么表达式为真。

    - 不会对变量进行替换扩展。因此，如果我们将该变量值引用其它的空值变量，也会返回 True。

    - 如果变量赋为空值 `VARIABLE-NAME :=`，会返回 False。

    ```
    $ cat Makefile
    foo =
    ifdef foo
    print += foo
    endif

    bar = $(foo)
    ifdef bar
    print += bar
    endif

    all:
        @echo $(print)

    $ make
    bar
    ```

 - `ifndef`：用来判断一个变量是否未定义。
