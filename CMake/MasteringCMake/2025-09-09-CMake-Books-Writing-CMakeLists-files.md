# Writing CMakeLists Files

## 注释

 1. 括号注释:

    ```
    #[[This is a bracket comment.
    It runs until the close bracket.]]

    or

    message("First Argument\n" #[[Bracket Comment]] "Second Argument")
    ```

 2. 行注释:

    ```
    # This is a line comment.
    message("First Argument\n" # This is a line comment :)
            "Second Argument") # This is a line comment.
    ```

## 变量

 1. 变量名区分大小写，并且只能包含**字母**，**数字** 和 **下划线**。

 2. **CMake** 自动定义了许多有用的变量，这些变量以 **CMAKE_** 开头。

 3. **CMake** 变量在内部存储时，均使用字符串类型，在运行时会被解释为其他类型。

 4. **set** 命令为设置变量值的命令。

    ```
    set(Foo "")      # 1 quoted arg    -> value is ""
    set(Foo a)       # 1 unquoted arg  -> value is "a"
    set(Foo "a b c") # 1 quoted arg    -> value is "a b c"
    set(Foo a b c)   # 3 unquoted args -> value is "a;b;c"
    ```

 5. 变量可以在命令参数中使用 **\$\{VAR\}** 来引用。

 6. 变量替换在展开未引用的参数之前进行，因此变量值将被分割成零个或多个参数，以取代原始未引用的参数。

    ```
    set(Foo a b c)    # 3 unquoted args -> value is "a;b;c"
    command(${Foo})   # unquoted arg replaced by a;b;c
                      # and expands to three arguments
    command("${Foo}") # quoted arg value is "a;b;c"
    set(Foo "")       # 1 quoted arg -> value is empty string
    command(${Foo})   # unquoted arg replaced by empty string
                      # and expands to zero arguments
    command("${Foo}") # quoted arg value is empty string
    ```

 7. 系统环境变量可以使用 **\$ENV\{VAR\}** 进行访问。

    ```
    set(MY_VAR $ENV{MY_ENV_VAR})
    message(${MY_VAR})
    ```

### 变量作用域

 1. 当设置一个变量时，它对当前的 **CMakeLists** 文件以及任何子目录的 **CMakeLists** 文件，任何调用的 **函数** 或 **宏**，以及任何 **include** 包含的文件都是可见的。

 2. 在子作用域中创建的任何新变量或对现有变量所做的任何更改都不会影响父作用域。

 3. 可以使用 **PARENT_SCOPE** 选项，在子作用域中修改父作用域中的变量。

    ```
    function(foo)
        set(test 2 PARENT_SCOPE)
    endfunction()

    set(test 1)
    foo()
    message(${test}) # test will now be 2 here
    ```

## 命令

 1. 命令由 **命令名称**，**左括号**，**用空白字符分隔的参数** 和 **右括号** 组成。

 2. **CMake** 对命令名称不区分大小写。

 3. 命令可以跨越多行，只要命令名称和开括号在同一条线上。

 4. 命令参数是用空格分隔的，并且区分大小写。

 5. 一个未引用的参数以任何不是双引号的字符开始，并在值内通过分号自动扩展为零或多个参数。

    ```
    command(a;b;c)       # 1 unquoted argument expands to 3
    ```

## 流量控制

### 条件语句

```
if(A)
  ...
elseif(B)
  ...
elseif(C)
  ....
else()
  ....
endif()
```

### 循环结构

 1. foreach 命令

    ```
    foreach(name A B C)
        message(name = ${name})
    endforeach()
    ```

 2. while 命令

    ```
    set(tick 0)
    while(tick LESS 10)
        message(tick = ${tick})
        math(EXPR tick "${tick} + 1")
    endwhile()
    ```

### 函数定义

 1. **function** 命令

    ```
    function(DetermineTime _time)
        set(${_time} "1:23:45" PARENT_SCOPE)
    endfunction()

    DetermineTime(current_time)

    if(DEFINED current_time)
        message(STATUS "The time is now: ${current_time}")
    endif()
    ```

 2. **macro** 命令

    **macro** 命令在调用时是在原地展开，并且可以支持可变参数列表。

    ```
    macro(assert TEST COMMENT)
        if(NOT ${TEST})
            message("Assertion failed: ${COMMENT}")

            if(${ARGC} MATCHES 3)
                file(APPEND ${ARGV2} "Assertion failed: ${COMMENT}")
            endif()
        endif()
    endmacro()

    find_library(FOO_LIB foo /usr/local/lib)
    assert(${FOO_LIB} "Unable to find library foo")
    ```

## 正则表达式

很多 **CMake** 命令支持正则表达式或可以接受正则表达式作为参数。
