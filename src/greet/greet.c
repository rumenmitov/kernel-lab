#include <linux/module.h>

#include "greet.h"

void greet(char *name)
{
	pr_info("Hello, %s!\n", name);
}

void goodbye(char *name)
{
	pr_info("Goodbye, %s!\n", name);
}
