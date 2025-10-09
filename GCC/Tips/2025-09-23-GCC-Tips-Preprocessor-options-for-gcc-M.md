# Preprocessor Options for GCC -M

## 参数详解

 - **-M**: 不是输出预编译过程的结果，而是输出一个用于 **Makefile** 的规则，该规则描述了这个源文件的依赖关系。

    ```
    $ gcc -M main.c
    main.o: main.c /usr/include/stdc-predef.h /usr/include/stdio.h \
     /usr/include/x86_64-linux-gnu/bits/libc-header-start.h \
     ...
     /usr/include/x86_64-linux-gnu/bits/floatn-common.h main.h
    ```

 - **-MM**: 与 **-M** 相似，只是不包含系统头文件。

    ```
    $ gcc -MM main.c
    main.o: main.c main.h
    ```

 - **-MF file**: 将依赖关系写到文件 **file** 中。

    ```
    $ gcc -MM main.c -MF main.d
    $ cat main.d
    main.o: main.c main.h
    ```

 - **-MG**: 若依赖的头文件不存在，仍继续编译，避免因缺失头文件报错。‌

 - **-MP**: 生成的依赖文件里面，对依赖规则中的所有 **.h** 依赖项都会在该文件中生成一个伪目标，其不依赖任何其他依赖项。

    ```
    $ gcc -c -MM -MP main.c
    main.o: main.c main.h
    main.h:
    ```

 - **-MT target / -MQ target**: 重新定义目标对象名。

    ```
    $ gcc -MM main.c -MT hello.o
    hello.o: main.c main.h
    ```

 - **-MD**: 等同于 **-M -MF file**。
