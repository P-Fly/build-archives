# Optimization Options for sections

## 背景

有时我们的程序会存在一些暂时使用不上的功能和函数，这些功能和函数往往会浪费我们的资源。这在使用静态库时，体现的更为严重。有时我们只使用了静态库仅有的几个功能，但是系统默认会自动把整个静态库全部链接到可执行程序中，造成可执行程序的资源开销大大增加。

## -ffunction-sections / -fdata-sections

**GCC** 的链接是以 **section** 作为最小的处理单元。只要 **section** 中的某个符号被引用，该 **section** 就会被加入到可执行程序中去。因此，我们在编译时可以使用 **-ffunction-sections** 和 **-fdata-sections** 将每个函数或符号创建为一个 **sections**，其中每个 **sections** 名与 **function** 或者 **data** 名保持一致。而在链接阶段，使用 **-Wl,–gc-sections** 指示链接器去掉不用的 **section** (**-wl**表示后面的参数传递给链接器)，这样就能显著减少最终程序的资源开销。

完整例子参见 [Sections Demo][1]

## 验证

### 比较大小

```
$ ls -l
-rwxr-xr-x 1 root root 16240 Oct 10 00:38 main_normal*
-rwxr-xr-x 1 root root 15992 Oct 10 00:38 main_sections*
```

可以看见 **main_sections** 二进制文件要小于 **main_normal** 的二进制文件。

### 分析 sections

```
$ readelf -t main_sections.o
  ...
Section Headers:
  ...
  [ 5] .text.fun_0
       PROGBITS         0000000000000000  0000000000000048  0
       0000000000000032 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [ 6] .rela.text.fun_0
       RELA             0000000000000000  00000000000004c0  24
       0000000000000048 0000000000000018  5                 8
       [0000000000000040]: INFO LINK
  [ 7] .text.fun_1
       PROGBITS         0000000000000000  000000000000007a  0
       0000000000000032 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [ 8] .rela.text.fun_1
       RELA             0000000000000000  0000000000000508  24
       0000000000000048 0000000000000018  7                 8
       [0000000000000040]: INFO LINK
  [ 9] .text.fun_2
       PROGBITS         0000000000000000  00000000000000ac  0
       0000000000000032 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [10] .rela.text.fun_2
       RELA             0000000000000000  0000000000000550  24
       0000000000000048 0000000000000018  9                 8
       [0000000000000040]: INFO LINK
  [11] .text.fun_3
       PROGBITS         0000000000000000  00000000000000de  0
       0000000000000032 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [12] .rela.text.fun_3
       RELA             0000000000000000  0000000000000598  24
       0000000000000048 0000000000000018  11                8
       [0000000000000040]: INFO LINK
  ...

$ readelf -t main_normal.o
  ...
Section Headers:
  ...
  [ 1] .text
       PROGBITS         0000000000000000  0000000000000040  0
       00000000000000dd 0000000000000000  0                 1
       [0000000000000006]: ALLOC, EXEC
  [ 2] .rela.text
       RELA             0000000000000000  0000000000000400  11
       0000000000000150 0000000000000018  1                 8
       [0000000000000040]: INFO LINK
  ...
```

可以看见 **main_sections.o** 每个函数都是一个独立的 **section**，**main_normal.o** 则共享一个默认的 **sections**。

### 分析elf文件

```
$ readelf -a main_normal | grep fun_
    22: 0000000000001149    50 FUNC    GLOBAL DEFAULT   16 fun_0
    26: 00000000000011ad    50 FUNC    GLOBAL DEFAULT   16 fun_2
    34: 00000000000011df    50 FUNC    GLOBAL DEFAULT   16 fun_3
    35: 000000000000117b    50 FUNC    GLOBAL DEFAULT   16 fun_1

$ readelf -a main_sections | grep fun_
    20: 0000000000001149    50 FUNC    GLOBAL DEFAULT   16 fun_0
    28: 000000000000117b    50 FUNC    GLOBAL DEFAULT   16 fun_3
```

可以看见在最终的目标文件 **main_sections** 中，未使用的函数并未被链接进最终的目标文件。

 [1]: https://github.com/P-Fly/build-archives/tree/master/GCC/Tips/demo/sections
