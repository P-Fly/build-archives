# Overall Options for GCC -x

## 参数详解

 - **-x language**: 忽略源文件的后缀名，以指定的语言进行语法分析。也就是说，你可以将C源文件定义为任意后缀名的文件，只要指定了 **-x c** 参数即可。它对参数后的所有源文件都起作用，除非重新指定语言或 **-x none** 关闭它。

 - **-x none**: 关闭该选项，参数后的文件使用后缀名自动识别语言。

## 例子

```
# 相当于 gcc main.c foo.c bar.c
gcc -x c main.pp foo.pp -x none bar.c
```
