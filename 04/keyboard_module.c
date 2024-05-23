#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init keyboard_init(void)
{
	pr_info("keyboard module : init\n");

	return 0;
}

static void __exit keyboard_exit(void)
{
	pr_info("keyboard module : cleanup\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("park sangjae");
MODULE_DESCRIPTION("keyboard module");

module_init(keyboard_init);
module_exit(keyboard_exit);
