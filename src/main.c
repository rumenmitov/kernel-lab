// SPDX-License-Identifier: GPL-2.0
#include <linux/module.h>
#include <linux/moduleparam.h>

#include "greet/greet.h"

static char *name = "world";

module_param(name, charp, 0);
MODULE_PARM_DESC(name, "The person to be greeted.");

static int __init dragonfire_init(void)
{
	greet(name);
	return 0;
}

static void __exit dragonfire_exit(void)
{
	goodbye(name);
}

module_init(dragonfire_init);
module_exit(dragonfire_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumen Mitov <rumenmitov@disroots.org>");
MODULE_DESCRIPTION("dragonfire - A Linux kernel module.");
