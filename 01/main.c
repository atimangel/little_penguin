#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_world_init(void)
{
	pr_info("Hello world !\n");

	return 0;
}

static void __exit hello_world_exit(void)
{
	pr_info("Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("park sangjae");
MODULE_DESCRIPTION("Hellow World module");

module_init(hello_world_init);
module_exit(hello_world_exit);
