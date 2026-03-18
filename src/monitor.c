// SPDX-License-Identifier: GPL-2.0

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/workqueue.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static int interval = 5;
module_param(interval, int, 0444);
MODULE_PARM_DESC(interval, "How often, in seconds, to poll the process list.");

static struct workqueue_struct *monitor_wq = NULL;
static struct task_struct *monitor_task = NULL;
struct work_struct monitor_work;

/**
 * @brief Work item that counts processes.
 */
static void __monitor_work_handler(struct work_struct *work)
{
	struct task_struct *task;
	uint32_t count = 0;

	(void)work;

	rcu_read_lock();
	for_each_process(task) {
		count++;
	}
	rcu_read_unlock();

	pr_info(KBUILD_MODNAME ": %u processes\n", count);
}

/**
 * @brief Every %interval seconds, add a work item to the workqueue.
 * @returns EINVAL on error.
 */
static int __monitor_loop(void *args)
{
	(void)args;

	INIT_WORK(&monitor_work, __monitor_work_handler);

	while (!kthread_should_stop()) {
		(void)msleep_interruptible(interval * 1000);

		if (!monitor_wq) {
			pr_err(KBUILD_MODNAME ": invalid workqueue!\n");
			return -EINVAL;
		}

		bool not_already_queued = queue_work(monitor_wq, &monitor_work);
		if (!not_already_queued) {
			pr_warn(KBUILD_MODNAME ": work already queued\n");
		}
	}

	return 0;
}

static int __init monitor_init(void)
{
	if (interval < 1) {
		pr_warn(KBUILD_MODNAME
			": interval parameter is less than 1, setting to 1\n");

		interval = 1;
	}

	monitor_wq = alloc_workqueue("monitor_wq", WQ_PERCPU, 1);
	if (!monitor_wq) {
		pr_err(KBUILD_MODNAME ": failed to create workqueue!\n");
		return -EINVAL;
	}

	monitor_task = kthread_run(__monitor_loop, NULL, "monitor_task");
	if (IS_ERR(monitor_task)) {
		pr_err(KBUILD_MODNAME ": failed to start monitor task!\n");
		return PTR_ERR(monitor_task);
	}

	return 0;
}

static void __exit monitor_exit(void)
{
	if (monitor_task) {
		(void)kthread_stop(monitor_task);
		monitor_task = NULL;
	}

	if (monitor_wq) {
		flush_workqueue(monitor_wq);
		destroy_workqueue(monitor_wq);
		monitor_wq = NULL;
	}
}

module_init(monitor_init);
module_exit(monitor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Rumen Mitov <rumenmitov@disroots.org>");
MODULE_DESCRIPTION(
	KBUILD_MODNAME
	" - A Linux kernel module that counts processes periodically.");
