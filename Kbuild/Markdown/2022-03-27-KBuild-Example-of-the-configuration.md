# 实例分析：内核配置命令

内核配置的相关编译命令如下，本文通过一些样例分析了内核的配置命令。

```
Configuration targets:
  config          - 基于文本界面更新当前配置
  nconfig         - 基于ncurses menu更新当前配置
  menuconfig      - 基于menu更新当前配置
  xconfig         - 基于QT的前端更新当前配置
  gconfig         - 基于GTK+的前端更新当前配置
  oldconfig       - 利用所提供的.config作为基础，更新当前配置
  localmodconfig  - 根据当前已加载模块列表和系统配置来生成配置文件
  localyesconfig  - 将所有可装载模块（LKM）都编译进内核
  defconfig       - 创建一份以当前系统架构为基础的默认设置文件
  savedefconfig   - 将当前配置保存为./defconfig
  allnoconfig     - 新配置除必须的选项外, 其它选项一律使用no
  allyesconfig    - 新配置尽量多地使用yes设置内核选项
  allmodconfig    - 新配置尽量多地使用m设置内核选项
  alldefconfig    - 新配置所有符号都设置为默认配置
  randconfig      - 新配置使用随机选项
  yes2modconfig   - 如果可能的话，将选项从yes更改为mod
  mod2yesconfig   - 如果可能的话，将选项从mod更改为yes
  listnewconfig   - 列出新选项
  helpnewconfig   - 列出新选项和帮助文本
  olddefconfig    - 与oldconfig相同，但在没有提示的情况下将新符号设置为默认值
  tinyconfig      - 配置尽可能小的内核
  testconfig      - 运行kconfig的单元测试
```

## **vexpress_defconfig**

**make vexpress_defconfig** 在 **TOP Makefile** 中执行的 **终极目标** 如下：

```
%config: outputmakefile scripts_basic FORCE
    $(Q)$(MAKE) $(build)=scripts/kconfig $@
```

该 **终极目标** 的执行可以分为 **outputmakefile**，**scripts_basic** 和 **命令行** 三部分进行分析：

 1. **outputmakefile**：

    ```
    outputmakefile:
        $(Q)if [ -f $(srctree)/.config -o \
             -d $(srctree)/include/config -o \
             -d $(srctree)/arch/$(SRCARCH)/include/generated ]; then \
            echo >&2 "***"; \
            echo >&2 "*** The source tree is not clean, please run 'make$(if $(findstring command line, $(origin ARCH)), ARCH=$(ARCH)) mrproper'"; \
            echo >&2 "*** in $(abs_srctree)";\
            echo >&2 "***"; \
            false; \
        fi
        $(Q)ln -fsn $(srctree) source
        $(call cmd,makefile)
        $(Q)test -e .gitignore || \
        { echo "# this is build directory, ignore it"; echo "*"; } > .gitignore
        endif
    ```

    该目标的主要功能如下：

    - 检查是否是干净的源码树。

    - 在 **output** 目录中，创建一个指向 **srctree** 的软链接。

    - 在 **output** 目录中，创建一个 **Makefile** 文件。该文件引用了 **srctree** 目录下的 **Makefile**。

    - 如果存在 **\.gitignore** 文件则修改它，使 **git** 忽略该目录下的所有文件。

 2. **scripts_basic**：

    ```
    # Basic helpers built in scripts/basic/
    PHONY += scripts_basic
    scripts_basic:
        $(Q)$(MAKE) $(build)=scripts/basic
    ```

    - 该规则使用了 **Makefile.include** 中定义的 **build** 函数，我们将其展开如下：

    ```
    # Defined in the Makefile.include
    build := -f $(srctree)/scripts/Makefile.build obj

    # Basic helpers built in scripts/basic/
    PHONY += scripts_basic
    scripts_basic:
        $(Q)$(MAKE) -f $(srctree)/scripts/Makefile.build obj=scripts/basic
    ```

    - 在 **Makefile.build** 中，会加载 **scripts/basic/Makefile** 文件。

    ```
    src := $(obj)

    ...

    # The filename Kbuild has precedence over Makefile
    kbuild-dir := $(if $(filter /%,$(src)),$(src),$(srctree)/$(src))
    kbuild-file := $(if $(wildcard $(kbuild-dir)/Kbuild),$(kbuild-dir)/Kbuild,$(kbuild-dir)/Makefile)
    include $(kbuild-file)
    ```

    - 在 **scripts/basic/Makefile** 中，通过变量 **hostprogs-always-y** 记录下了需要编译的目标文件。

    ```
    hostprogs-always-y  += fixdep
    ```

    - 在 **Makefile.build** 中，会加载 **Makefile.lib** 文件。该文件会将变量 **hostprogs-always-y** 分解到 **hostprogs** 和 **always-y** 中。

    ```
    # hostprogs-always-y += foo
    # ... is a shorthand for
    # hostprogs += foo
    # always-y  += foo
    hostprogs += $(hostprogs-always-y) $(hostprogs-always-m)
    always-y += $(hostprogs-always-y) $(hostprogs-always-m)
    ```

    - 因为存在变量 **hostprogs**，因此在 **Makefile.build** 中，会加载 **Makefile.host** 文件。

    ```
    # Do not include hostprogs rules unless needed.
    # $(sort ...) is used here to remove duplicated words and excessive spaces.
    hostprogs := $(sort $(hostprogs))
    ifneq ($(hostprogs),)
    include $(srctree)/scripts/Makefile.host
    endif
    ```

    - 最终，我们使用 **Makefile.host** 中的规则完成编译工作。

    ```
    # C code
    # Executables compiled from a single .c file
    host-csingle    := $(foreach m,$(hostprogs), \
        $(if $($(m)-objs)$($(m)-cxxobjs),,$(m)))

    ...

    # Create executable from a single .c file
    # host-csingle -> Executable
    quiet_cmd_host-csingle  = HOSTCC  $@
        cmd_host-csingle    = $(HOSTCC) $(hostc_flags) $(KBUILD_HOSTLDFLAGS) -o $@ $< \
        $(KBUILD_HOSTLDLIBS) $(HOSTLDLIBS_$(target-stem))
    $(host-csingle): $(obj)/%: $(src)/%.c FORCE
        $(call if_changed_dep,host-csingle)
    ```

 ![make scripts_basic][1]

 3. **命令行**：

    - 命令行展开后会执行 **scripts/kconfig/Makefile** 下的 **\%\_defconfig** 规则。

    ```
    %_defconfig: $(obj)/conf
        $(Q)$< $(silent) --defconfig=arch/$(SRCARCH)/configs/$@ $(Kconfig)
    ```

    - 该规则需要首先生成依赖文件 **conf**。与 **scripts_basic** 类似，在 **scripts/kconfig/Makefile** 中，使用变量 **hostprogs** 记录下需要生成的文件。

    ```
    # object files used by all kconfig flavours
    common-objs := confdata.o expr.o lexer.lex.o menu.o parser.tab.o \
               preprocess.o symbol.o util.o

    ...

    hostprogs   += conf
    conf-objs   := conf.o $(common-objs)
    ```

    - 因为依赖文件 **conf** 是由多个 **\.c** 源文件构成的，因此在 **Makefile.host** 中，需要使用 **host-cmulti** 规则将 **conf** 作为目标文件。

    ```
    # C executables linked based on several .o files
    host-cmulti := $(foreach m,$(hostprogs),\
               $(if $($(m)-cxxobjs),,$(if $($(m)-objs),$(m))))

    ...

    # Link an executable based on list of .o files, all plain c
    # host-cmulti -> executable
    quiet_cmd_host-cmulti   = HOSTLD  $@
          cmd_host-cmulti   = $(HOSTCC) $(KBUILD_HOSTLDFLAGS) -o $@ \
                  $(addprefix $(obj)/, $($(target-stem)-objs)) \
                  $(KBUILD_HOSTLDLIBS) $(HOSTLDLIBS_$(target-stem))
    $(host-cmulti): FORCE
        $(call if_changed,host-cmulti)
    $(call multi_depend, $(host-cmulti), , -objs)
    ```

    这儿的 **multi_depend** 是一个在 **Makefile.lib** 中定义的 **命令包**，用来生成依赖文件列表：

    ```
    # Useful for describing the dependency of composite objects
    # Usage:
    #   $(call multi_depend, multi_used_targets, suffix_to_remove, suffix_to_add)
    define multi_depend
    $(foreach m, $(notdir $1), \
        $(eval $(obj)/$m: \
        $(addprefix $(obj)/, $(foreach s, $3, $($(m:%$(strip $2)=%$(s)))))))
    endef
    ```

    展开后，目标文件 **conf** 的规则如下：

    ```
    scripts/kconfig/conf: scripts/kconfig/conf.o scripts/kconfig/confdata.o ...
    ```

    此时，对于该目标文件的依赖列表中的每个 **\.o** 文件，使用如下规则进行编译：

    ```
    # Object (.o) files compiled from .c files
    host-cobjs  := $(sort $(foreach m,$(hostprogs),$($(m)-objs)))

    ...

    # Create .o file from a single .c file
    # host-cobjs -> .o
    quiet_cmd_host-cobjs    = HOSTCC  $@
          cmd_host-cobjs    = $(HOSTCC) $(hostc_flags) -c -o $@ $<
    $(host-cobjs): $(obj)/%.o: $(src)/%.c FORCE
        $(call if_changed_dep,host-cobjs)
    ```

    - 最终，**conf** 工具生成完毕后，**make vexpress_defconfig** 执行的命令如下：

    ```
    scripts/kconfig/conf  --defconfig=arch/arm/configs/vexpress_defconfig Kconfig
    ```

## menuconfig

**make menuconfig** 在 **TOP Makefile** 中执行的 **终极目标** 与 **make vexpress_defconfig** 一致：

```
%config: outputmakefile scripts_basic FORCE
    $(Q)$(MAKE) $(build)=scripts/kconfig $@
```

在 **scripts/kconfig/Makefile** 中，通过 **命令包** 生成了 **menuconfig** 的规则。

```
define config_rule
PHONY += $(1)
$(1): $(obj)/$($(1)-prog)
    $(Q)$$< $(silent) $(Kconfig)

PHONY += build_$(1)
build_$(1): $(obj)/$($(1)-prog)
endef

$(foreach c, config menuconfig nconfig gconfig xconfig, $(eval $(call config_rule,$(c))))
```

展开后，目标文件 **menuconfig** 的规则如下：

```
PHONY += menuconfig
menuconfig: $(obj)/mconf
    $(Q)$$< $(silent) $(Kconfig)

PHONY += build_menuconfig
build_menuconfig: $(obj)/mconf
```

生成 **mconf** 的流程与 **make vexpress_defconfig** 中生成 **conf** 的流程几乎一致。

最终 **make menuconfig** 执行的命令如下：

```
scripts/kconfig/mconf Kconfig
```

## savedefconfig

**make savedefconfig** 在 **TOP Makefile** 中执行的 **终极目标** 与 **make vexpress_defconfig** 一致：

```
%config: outputmakefile scripts_basic FORCE
    $(Q)$(MAKE) $(build)=scripts/kconfig $@
```

根据前文的分析，会使用 **scripts/kconfig/Makefile** 中的规则完成编译工作：

```
savedefconfig: $(obj)/conf
    $(Q)$< $(silent) --$@=defconfig $(Kconfig)
```

生成 **conf** 的流程与 **make vexpress_defconfig** 中生成 **conf** 的流程一致。

最终 **make savedefconfig** 执行的命令如下：

```
scripts/kconfig/conf  --savedefconfig=defconfig Kconfig
```

 [1]: ./images/make_scripts_basic.jpg
