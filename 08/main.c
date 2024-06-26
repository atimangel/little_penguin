#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");

static ssize_t myfd_read (struct file *fp, char __user *user, size_t size,
		loff_t *offs);
static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		loff_t *offs);

static struct file_operations myfd_fops = {
	.owner = THIS_MODULE,
	.read = myfd_read,
	.write = myfd_write
};

static struct miscdevice myfd_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "reverse",
	.fops = &myfd_fops
};

static char str[PAGE_SIZE];

static int __init myfd_init (void)
{
	int retval;

	retval = misc_register(&myfd_device);
	return retval;
}

static void __exit myfd_cleanup (void)
{
	misc_deregister(&myfd_device);
}

ssize_t myfd_read (struct file *fp, char __user *user, size_t size,
		loff_t *offs)
{
	size_t t;
	size_t i;
	ssize_t retval = 0;
	char *tmp;

	tmp = kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!tmp) {
		retval = -ENOMEM;
		goto out;
	}

	for (t = strlen(str), i = 0; t > 0; t--, i++)
		tmp[i] = str[t - 1];

	tmp[i] = '\0';
	retval = simple_read_from_buffer(user, size, offs, tmp, i);
	kfree(tmp);

out:
	return retval;
}

ssize_t myfd_write (struct file *fp, const char __user *user, size_t size,
	loff_t *offs)
{
	ssize_t res;

	res = simple_write_to_buffer(str, size, offs, user, size);
	str[res] = '\0';

	return res;
}

module_init(myfd_init);
module_exit(myfd_cleanup);
