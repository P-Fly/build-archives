# **Makefile.compiler** 详细分析

## 主要功能

**Makefile.compiler** 文件主要提供了 **GCC** 的一些访问接口，比如检查版本，检查是否支持某些特定选项的功能。

## 文件导入

**Top Makefile** 中的变量 **no-compiler-targets** 定义了不需要编译器参与的目标。然后通过 **filter** 和 **filter-out** 语句判断编译目标中是否有且仅有 **no-compiler-targets** 中的目标。如果是，则 **need-compiler** 为 **空**，否则 **need-compiler** 为 **1**。

当 **need-compiler** 为 **1** 时，导入 **Makefile.compiler** 文件。

```
# 定义不需要编译器参与的目标
no-compiler-targets := $(no-dot-config-targets) install dtbs_install \
            headers_install modules_install kernelrelease image_name

...

# 设置 need-compiler 的默认值
need-compiler   := 1

...

# 判断编译目标是否需要编译器参与
ifneq ($(filter $(no-compiler-targets), $(MAKECMDGOALS)),)
    ifeq ($(filter-out $(no-compiler-targets), $(MAKECMDGOALS)),)
        need-compiler :=
    endif
endif

...

# 如果需要编译器参与则加载 Makefile.compiler 文件
ifdef need-compiler
include $(srctree)/scripts/Makefile.compiler
endif
```

## **try-run** 的实现

在 **Makefile.compiler** 文件中，几乎所有的函数都是基于 **try-run** 来实现的，因此我们可以先分析该函数的实现。

从源码可以看出，该函数的主要逻辑处理如下：

 1. 建立一个临时文件。

 2. 使用 **$1** 的执行结果作为逻辑判断，如果成功则返回 **$2**，失败则返回 **$3**。

 3. 删除创建的临时文件。

因此 **try-run** 的目的就是根据 **CC** 的执行结果，返回不同的选项。

```
# try-run
# Usage: option = $(call try-run, $(CC)...-o "$$TMP",option-ok,otherwise)
# Exit code chooses option. "$$TMP" serves as a temporary file and is
# automatically cleaned up.
try-run = $(shell set -e;       \
    TMP=$(TMPOUT)/tmp;      \
    mkdir -p $(TMPOUT);     \
    trap "rm -rf $(TMPOUT)" EXIT;   \
    if ($(1)) >/dev/null 2>&1;  \
    then echo "$(2)";       \
    else echo "$(3)";       \
    fi)
```

## 主要功能函数

 - **cc-cross-prefix**：用于检查路径中是否存在一个具有所列出的前缀的 **$(CC)**。

 - **as-option** 用于检查 **$(CC)** 在用于汇编文件时，是否支持给定选项。如果不支持第一个选项，则可以指定一个可选的第二选项。

 - **as-instr**：检查汇编程序是否报告了特定的指令。

 - **cc-option**：用于检查 **$(CC)** 是否支持给定的选项，如果不支持第一个选项，则可以指定一个可选的第二选项。

 - **cc-option-yn**：用于检查 **$(CC)** 是否支持给定的选项，如果支持则返回 **y**，否则返回 **n**。

 - **cc-disable-warning**：检查 **$(CC)** 是否支持给定的警告。

- **cc-ifversion**：测试 **$(CC)** 的版本。

- **ld-option**：用于检查 **$(LD)** 是否支持所提供的选项。如果不支持第一个选项，则可以指定一个可选的第二选项。

- **ld-ifversion**：测试 **$(LD)** 的版本。

大部分接口函数在内核官方文档中有详细的描述：**[Linux Kernel Makefiles][1]**。

 [1]: https://www.kernel.org/doc/html/latest/kbuild/makefiles.html#cc-support-functions
