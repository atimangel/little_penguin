#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/err.h>
#include <linux/types.h>
#include <linux/mutex.h>

DEFINE_MUTEX(foo_mutex);
char	msg[PAGE_SIZE];

struct dentry *dir;

static ssize_t id_read(struct file *filp, char __user *to_user,
		size_t count, loff_t *position)
{
	char *my_id = "snpark";

	return simple_read_from_buffer(to_user, count, position, my_id,
			strlen(my_id));
}

static ssize_t id_write(struct file *flip, const char __user *from_user,
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

static struct file_operations id_fops = {
	.read = id_read,
	.write = id_write
};

static ssize_t foo_read(struct file *flip, char __user *to_user,
		size_t count, loff_t *position)
{
	int ret = -EINVAL;

	mutex_lock(&foo_mutex);

	ret = simple_read_from_buffer(to_user, count, position, msg,
		       	strlen(msg));

	mutex_unlock(&foo_mutex);
	return ret;
}

static ssize_t foo_write(struct file *flip, const char __user *from_user,
		size_t count, loff_t *position)
{
	ssize_t len;

	mutex_lock(&foo_mutex);

	len = simple_write_to_buffer(msg, PAGE_SIZE, position, from_user,
			count);
	pr_info("%lu\n", len);
	msg[len] = '\0';

	mutex_unlock(&foo_mutex);

	return len;
}

static struct file_operations foo_fops = {
	.read = foo_read,
	.write = foo_write
};

static int __init hello_world_init(void)
{
	if (!(dir = debugfs_create_dir("fortytwo", NULL))) {
		pr_err("fourtytwo debug directory creation failed\n");
		return -ENODEV;
	}

	if (!debugfs_create_file("id", 0666, dir, NULL, &id_fops))
		pr_err("id file debug create fail\n");

	if (BITS_PER_LONG == 64)
		debugfs_create_u64("jiffies", 0444, dir, (u64*) &jiffies);
	else
		debugfs_create_u32("jiffies", 0444, dir, (u32*) &jiffies);
	
	if (!debugfs_create_file("foo", 0644, dir, NULL, &foo_fops))
		pr_err("foo file debug create fail\n");

	pr_info("Hello world !\n");
	return 0;
}

static void __exit hello_world_exit(void)
{
	debugfs_remove_recursive(dir);
	pr_info("Cleaning up module.\n");
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("park sangjae");
MODULE_DESCRIPTION("Hellow World module");
MODULE_VERSION("1.0");

module_init(hello_world_init);
module_exit(hello_world_exit);
