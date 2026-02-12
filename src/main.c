// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>

static int __init dragonfire_init(void)
{
	pr_info("Hello, world!\n");
	return 0;
}

static void __exit dragonfire_exit(void)
{
	pr_info("Goodbye, world!\n");
}

module_init(dragonfire_init);
module_exit(dragonfire_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumen Mitov");
MODULE_DESCRIPTION("dragonfire - A Linux kernel module.");
