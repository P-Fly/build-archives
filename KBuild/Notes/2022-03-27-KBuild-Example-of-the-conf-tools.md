# 实例分析：配置管理工具 **conf**

工具 **conf** 的源码位于内核 **scripts\kconfig** 的目录下，用于管理内核的配置选项。

该工具一般会使用即可，因此本文并不准备总结源码的所有细节，只是以配置命令 **make vexpress_defconfig** 为例，简要分析了该工具执行的过程和结果。

1. 当执行 **make vexpress_defconfig** 时，**Makefile** 最终的执行结果如下：

```
scripts/kconfig/conf  --defconfig=arch/arm/configs/vexpress_defconfig Kconfig
```

2. 参看源码，首先进行参数解析。

```
while ((opt = getopt_long(ac, av, "s", long_opts, NULL)) != -1) {
    ...
}
```

3. 使用函数 **conf_parse** 解析内核源码树下的所有 **Kconfig** 文件。

```
name = av[optind];
conf_parse(name);
```

4. 读取指定的配置文件。

```
switch (input_mode) {
case defconfig:
    if (conf_read(defconfig_file)) {
        ...
    }
    break;
...
}
```

5. 将新的选项设置为默认值。

```
switch (input_mode) {
...
case defconfig:
    conf_set_all_new_symbols(def_default);
    break;
...
}
```

6. 更新 **.config** 文件。

```
if (!no_conf_write && conf_write(NULL)) {
    fprintf(stderr, "\n*** Error during writing of the configuration.\n\n");
    exit(1);
}
```

7. 更新头文件 **include/config/auto.conf** 和 **include/generated/autoconf.h**。

```
/*
 * Create auto.conf if it does not exist.
 * This prevents GNU Make 4.1 or older from emitting
 * "include/config/auto.conf: No such file or directory"
 * in the top-level Makefile
 *
 * syncconfig always creates or updates auto.conf because it is
 * used during the build.
 */
if (conf_write_autoconf(sync_kconfig) && sync_kconfig) {
    fprintf(stderr,
        "\n*** Error during sync of the configuration.\n\n");
    return 1;
}
```

*备注：当前的代码中，会在 **conf_write_autoconf** 中通过 **conf_touch_deps** 函数，将每个配置宏在 **include/config/** 目录下生成单独的一个空文件，目前还不清楚这么做的目的是什么。*

最后，总结下生成的几个配置文件，虽然它们都是记录的内核配置信息，但是其使用对象并不相同：

 - **.config**：由 **conf** 工具生成，记录了所有配置选项的状态。

 - **include/config/auto.conf**：记录了所有 **Enable** 的配置选项，提供给 **Makefile** 文件使用。

 - **include/generated/autoconf.h**：记录了所有 **Enable** 的配置选项，给内核源代码使用。
