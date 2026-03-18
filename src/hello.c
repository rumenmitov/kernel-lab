// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>

static int __init hello_init(void)
{
	pr_info("Hello, world!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Goodbye, world!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumen Mitov <rumenmitov@disroots.org>");
MODULE_DESCRIPTION(KBUILD_MODNAME " - A Linux kernel module.");
