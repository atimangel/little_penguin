#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define MISC_DEVICE_NAME "fortytwo"
#define MY_ID "snpark"
#define MY_ID_LEN 6 

static ssize_t misc_read(struct file *filp, char __user *to_user,
		size_t count, loff_t *position)
{
	char *my_id = "snpark";

	return simple_read_from_buffer(to_user, count, position, my_id,
			strlen(my_id));
}

static ssize_t misc_write(struct file *flip, const char __user *from_user,
		size_t count, loff_t *position)
{
	ssize_t ret;
	char *my_id = "snpark";
	char *to_file = kmalloc(count + 1, GFP_KERNEL);

	if (!to_file)
		return -ENOMEM;

	if (strlen(my_id) != count -1){
		ret = -EINVAL;
		goto out;
	}

	ret = simple_write_to_buffer(to_file, count, position, from_user,
		       	count);

	if (strncmp(to_file, my_id, strlen(my_id)) != 0) {
		ret = -EINVAL;
		goto out;
	}

out:
	kfree(to_file);
	return ret;
}

const struct file_operations misc_fops = {
	.read = misc_read,
	.write = misc_write,
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MISC_DEVICE_NAME,
	.fops = &misc_fops,
};

static int __init hello_world_init(void)
{
	int ret = misc_register(&my_misc_device);
	pr_info("Hello world!; return %d\n", ret);

	if (ret < 0)
		return -EFAULT;
	return 0;
}

static void __exit hello_world_exit(void)
{
	misc_deregister(&my_misc_device);
	pr_info("Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("park sangjae");

module_init(hello_world_init);
module_exit(hello_world_exit);
