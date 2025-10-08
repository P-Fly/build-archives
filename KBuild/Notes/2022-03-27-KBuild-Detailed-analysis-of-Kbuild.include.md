# **Makefile.include** 详细分析

## 主要功能

**Makefile.include** 文件提供了许多通用函数和变量的定义，这些定义被 **Makefile.\*** 和 **Top Makefile** 所使用。该脚本参与了整个内核编译的过程，是编译的核心脚本之一。

## **build** 函数

**build** 函数的主要功能是将编译流程传递给 **obj** 指定的文件夹，再由该目录下的 **Kbuild** 或者 **Makefile** 文件进行编译。

```
build := -f $(srctree)/scripts/Makefile.build obj
```

比如 **xxx_defconfig** 的规则，在 **Top Makefile** 中定义如下：

```
%config: outputmakefile scripts_basic FORCE
    $(Q)$(MAKE) $(build)=scripts/kconfig $@
```

我们按照 **build** 函数展开后变为：

```
%config: outputmakefile scripts_basic FORCE
    $(Q)$(MAKE) -f $(srctree)/scripts/Makefile.build obj=scripts/kconfig xxx_defconfig
```

此时，**Top Makefile** 通过 **\$(MAKE) -f** 参数指定读取 **$(srctree)/scripts/Makefile.build** 文件，来继续处理对应的规则。

在 **$(srctree)/scripts/Makefile.build** 中，会加载 **scripts/kconfig** 目录中的 **Kbuild** 或者 **Makefile** 文件：

```
src := $(obj)

...

# The filename Kbuild has precedence over Makefile
kbuild-dir := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
kbuild-file := $(if $(wildcard $(kbuild-dir)/Kbuild),$(kbuild-dir)/Kbuild,$(kbuild-dir)/Makefile)
include $(kbuild-file)
```

最终，在 **scripts/kconfig/Makefile** 中找到终极目标：

```
%_defconfig: $(obj)/conf
    $(Q)$< $(silent) --defconfig=arch/$(SRCARCH)/configs/$@ $(Kconfig)
```

与 **build** 相似的函数还有 **dtbinst** 和 **clean**。

需要注意：不是所有的 **目标文件** 都在 **\$build** 指向的子目录中。当未指定 **目标文件** 时，**Makefile.build** 中的 **__build** 就成为了默认的 **目标文件**，按照 **Makefile.build** 中定义的规则执行命令。

## **cmd** 函数

**cmd** 函数负责执行具体的命令，同时输出该命令的辅助信息。

```
cmd = @set -e; $(echo-cmd) $($(quiet)redirect) $(cmd_$(1))
```

我们以规则 **outputmakefile** 中的命令 **makefile**为例：

```
outputmakefile:
    ...
    $(call cmd,makefile)
    ...
endif
```

该函数展开如下：

```
@set -e; $(echo-cmd) $($(quiet)redirect) $(cmd_makefile)
```

 - **\$(echo-cmd)**： 负责输出该命令的辅助信息，其展开如下：

    ```
    echo-cmd = $(if $($(quiet)cmd_makefile),\
        echo '  $(call escsq,$($(quiet)cmd_makefile))$(echo-why)';)
    ```

    - **quiet** 记录了构建信息的详细程度，其值为 **quite_** 或 **空**。

    - 整个语句表示为通过 **echo** 打印 **quiet_cmd_makefile** 或 **cmd_makefile** 的命令信息。

    - 当 **make V = 2** 时，还会通过 **\$(echo-why)** 打印构建原因。

 - **\$(\$(quiet)redirect)**：当输入 **make -s** 时，屏蔽输出。

    ```
    # sink stdout for 'make -s'
           redirect :=
     quiet_redirect :=
    silent_redirect := exec >/dev/null;
    ```

 - **\$(cmd_makefile)**：需要执行的命令。

总结：

当我们需要自定义一个命令 **foo** 时，可以按照下面的例子去定义和使用。

```
quiet_cmd_foo = Print foo info
      cmd_foo = foo command

...

$(call cmd,foo)
```

## **if_changed** 函数

**if_changed** 函数是当仁不让的核心函数，它的功能是当 **依赖文件** 有更新或者 **命令行** 有改动时，重新构建 **目标文件**。并且在 **目标文件** 被构建时，将 **命令行指令** 记录到对应的 **.\*.cmd** 文件中。

```
if_changed = $(if $(if-changed-cond),                                    \
    $(cmd);                                                              \
    printf '%s\n' 'cmd_$@ := $(make-cmd)' > $(dot-target).cmd, @:)
```

我们以一个自定义规则 **foo** 为例：

```
quiet_cmd_create_foo = create foo
    cmd_create_foo = touch foo

foo: FORCE
    $(call if_changed,create_foo)

targets += foo
```

**\$(if-changed-cond)** 负责检查 **依赖文件** 和 **命令行** 是否有变化，它由三个子函数构成，任意一个子函数运行结果为非空，都会重新构建 **目标文件**：

```
if-changed-cond = $(newer-prereqs)$(cmd-check)$(check-FORCE)
```

 - **\$(newer-prereqs)**：

    ```
    # Find any prerequisites that are newer than target or that do not exist.
    # (This is not true for now; $? should contain any non-existent prerequisites,
    # but it does not work as expected when .SECONDARY is present. This seems a bug
    # of GNU Make.)
    # PHONY targets skipped in both cases.
    newer-prereqs = $(filter-out $(PHONY),$?)
    ```

    该函数会列出去除了伪目标后，比 **目标文件** 新的 **依赖文件** 列表。

 - **\$(cmd-check)**：

    ```
    # Check if both commands are the same including their order. Result is empty
    # string if equal. User may override this check using make KBUILD_NOCMDDEP=1
    cmd-check = $(filter-out $(subst $(space),$(space_escape),$(strip $(cmd_$@))), \
                             $(subst $(space),$(space_escape),$(strip $(cmd_$1))))
    ```

    该函数会检查 **命令行** 是否有变化。

    - **\$(cmd_\$\@)** 替换后为 **\$(cmd_foo)**，该变量从文件 **.foo.cmd** 中获取，记录的是 **上一次的命令行指令**。该变量被下列语句加载到 **Makefile** 中。

        ```
        # read saved command lines for existing targets
        existing-targets := $(wildcard $(sort $(targets)))

        -include $(foreach f,$(existing-targets),$(dir $(f)).$(notdir $(f)).cmd)
        ```

    - **\$(cmd_\$1)** 替换后为 **\$(cmd_create_foo)**，展开后为 **当前的命令行指令**。

 - **\$(check-FORCE)**：

    ```
    # It is a typical mistake to forget the FORCE prerequisite. Check it here so
    # no more breakage will slip in.
    check-FORCE = $(if $(filter FORCE, $^),,$(warning FORCE prerequisite is missing))
    ```

    因为当前我们是由 **if_changed** 函数来判断 **依赖关系**，因此，**目标文件** 需要强制设置为 **FORCE**。该函数用于检查是否遗漏了 **FORCE** 关键字。

与 **if_changed** 函数相似的函数还有 **if_changed_dep** 和 **if_changed_rule**。

**if_changed** 函数在内核官方文档中有详细的描述：**[Linux Kernel Makefiles][1]**。

 [1]: https://www.kernel.org/doc/html/latest/kbuild/makefiles.html#command-change-detection
