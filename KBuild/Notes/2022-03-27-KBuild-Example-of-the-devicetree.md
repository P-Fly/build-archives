# 实例分析：编译设备树

本文以编译命令 **make ARCH=arm dtbs** 为例，简要分析了设备树的编译过程和编译结果。

在 **TOP Makefile** 中执行的 **终极目标** 如下：

```
PHONY += dtbs dtbs_install dtbs_check
dtbs: include/config/kernel.release scripts_dtc
    $(Q)$(MAKE) $(build)=$(dtstree)
```

该 **终极目标** 的执行可以分为 **include/config/kernel.release**，**scripts_dtc** 和 **命令行** 三部分：

 1. **include/config/kernel.release**

```
# Store (new) KERNELRELEASE string in include/config/kernel.release
include/config/kernel.release: FORCE
    $(call filechk,kernel.release)
```

该目标通过 **filechk** 函数，检测内核头文件 **include/config/kernel.release** 是否为最新，如果该文件不存在或者非最新，则重新生成该头文件。

 2. **scripts_dtc**

```
PHONY += scripts_dtc
scripts_dtc: scripts_basic
    $(Q)$(MAKE) $(build)=scripts/dtc
```

该目标文件的执行与依赖文件 **scripts_basic** 的方式几乎一致，可以参考文章 [实例分析：内核配置命令][1]。

最终生成了设备树的编译工具 **dtc** 和 **fdtoverlay**。

 3. **命令行**

    - 在 **TOP Makefile** 中，**dtstree** 指向了内核设备树的存储路径。

    ```
    ifneq ($(wildcard $(srctree)/arch/$(SRCARCH)/boot/dts/),)
    dtstree := arch/$(SRCARCH)/boot/dts
    endif
    ```

    - 假设我们使用的配置文件为 **vexpress_defconfig**，那么 **CONFIG_ARCH_VEXPRESS=y**。根据 **arch/arm/boot/dts/Makefile** 中的如下配置，使用变量 **dtb-y** 记录需要生成的设备树。

    ```
    dtb-$(CONFIG_ARCH_VEXPRESS) += \
        vexpress-v2p-ca5s.dtb \
        vexpress-v2p-ca9.dtb \
        vexpress-v2p-ca15-tc1.dtb \
        vexpress-v2p-ca15_a7.dtb
    ```

    - 在 **Makefile.lib** 中，使用 **always-y** 记录需要生成的设备树。

    ```
    always-y            += $(dtb-y)
    ```

    - 最终，在 **Makefile.lib** 中，使用如下规则编译设备树。

    ```
    quiet_cmd_dtc = DTC     $@
    cmd_dtc = $(HOSTCC) -E $(dtc_cpp_flags) -x assembler-with-cpp -o $(dtc-tmp) $< ; \
        $(DTC) -o $@ -b 0 \
            $(addprefix -i,$(dir $<) $(DTC_INCLUDE)) $(DTC_FLAGS) \
            -d $(depfile).dtc.tmp $(dtc-tmp) ; \
        cat $(depfile).pre.tmp $(depfile).dtc.tmp > $(depfile)

    $(obj)/%.dtb: $(src)/%.dts $(DTC) FORCE
        $(call if_changed_dep,dtc)
    ```

 [1]: https://github.com/P-Fly/build-archives/blob/master/Kbuild/Markdown/2022-03-27-KBuild-Example-of-the-configuration.md
