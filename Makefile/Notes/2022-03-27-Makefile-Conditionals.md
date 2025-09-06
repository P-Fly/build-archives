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

 - **ifeq**：用来判断参数是否相等。

    ```
    ifeq ($(strip $(foo)),)
    TEXT-IF-EMPTY
    endif
    ```

 - **ifneq**：用来判断参数是否不相等。

 - **ifdef**：用来判断一个变量是否定义。

    - 如果变量的值非空，那么表达式为真。

   - **ifdef** 只测试变量是否有值，不会对变量进行替换展开来判断变量的值是否为空。

    ```
    例1：

    bar =
    foo = $(bar)
    ifdef foo
    frobozz = yes
    else
    frobozz = no
    endif

    结果是：frobozz = yes

    例2：

    foo =
    ifdef foo
    frobozz = yes
    else
    frobozz = no
    endif

    结果是：frobozz = no
    ```

 - **ifndef**：用来判断一个变量是否未定义。
