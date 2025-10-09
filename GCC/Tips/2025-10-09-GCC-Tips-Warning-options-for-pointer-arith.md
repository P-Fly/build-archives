# Warning Options for pointer-arith

最近在编译代码时遇到了 **-Wpointer-arith** 警告。

该警告是指在对 **函数指针** 或 **void\*指针** 进行 **算术运算** 时，编译器会触发此警告。

**GNU C** 会将这些类型的大小指定为 **1**，以方便使用 **void\*指针** 和 **函数指针** 进行计算。

可以通过 **-pedantic-errors** 将该警告升级为错误。

完整例子参见 [-Wpointer-arith Demo][1]

最终修复方式如下:

```
#define container_of(ptr, type, member) ({          \
    void *__mptr = (void *)(ptr);                   \
    ((type *)((char*)__mptr - offsetof(type, member))); })
```

 [1]: https://github.com/P-Fly/build-archives/tree/master/GCC/Tips/demo/-Wpointer-arith
