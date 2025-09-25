# Warning Options

| 选项 | 说明 |
| :- | :- |
| -Wimplicit-int | 警告没有指定类型的声明 |
| -Wimplicit-function-declaration | 警告在声明之前就使用的函数 |
| -Wimplicit | 同 -Wimplicit-int 和 -Wimplicit-function-declaration |
| -Wpointer-arith | 对函数指针或者 void\* 类型的指针进行算术操作时给出警告 |
| -Wredundant-decls | 如果在同一个可见域某定义多次声明，编译器就发出警告，即使这些重复声明有效并且毫无差别 |
| -Wreturn-type | 如果函数定义了返回类型，而默认类型是 int 型，编译器就发出警告。同时警告那些不带返回值的 return 语句，如果他们所属的函数并非 void 类型 |
| -Wshadow | 当一个局部变量遮盖住了另一个局部变量，或者全局变量时，给出警告 |
| -Wunused | 如果某个局部变量除了声明就没再使用，或者声明了静态函数但是没有定义，或者某条语句的运算结果显然没有使用，编译器就发出警告 |
| -Wswitch | 如果某条 switch 语句的参数属于枚举类型，但是没有对应的 case 语句使枚举元素，编译器就发出警告(default 语句的出现能够防止这个警告)。超出枚举范围 case 语句同样会导致这个警告 |
| -Wnested-externs | 如果某 extern 声明出现在函数内部，编译器就发出警告 |
| -Wall | 使能所有警告 |
