#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/spinlock.h>
#include <linux/nsproxy.h>
#include <linux/ns_common.h>
#include <linux/poll.h>
#include <linux/mnt_namespace.h>
#include "./mount.h"


static struct proc_dir_entry *entry;

static int mymounts_proc_show(struct seq_file *m, void *v)
{
	struct mount *mnt;

	list_for_each_entry(mnt, &current->nsproxy->mnt_ns->list, mnt_list) {
		struct path mnt_path =  {
			.dentry = mnt->mnt.mnt_root,
			.mnt = &mnt->mnt
		};
		struct super_block *sb = mnt_path.dentry->d_sb;

		if (!strcmp(mnt->mnt_devname, "rootfs"))
			continue;
		if (sb->s_op->show_devname)
			sb->s_op->show_devname(m, mnt_path.dentry);
		else
			seq_puts(m, mnt->mnt_devname ? mnt->mnt_devname : "none");
		seq_putc(m, ' ');
		seq_path(m, &mnt_path, " \t\n\\");
		seq_putc(m, '\n');
	}
	return 0;
}

static int mymounts_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, mymounts_proc_show, NULL);
}

static const struct proc_ops mymounts_proc_fops= {
	.proc_open = mymounts_proc_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init my_init(void)
{
	entry = proc_create("mymounts", 0, NULL, &mymounts_proc_fops);
	if (entry == NULL) {
		pr_err("failed to create /proc/mymounts file\n");
		return -ENOMEM;
	}

	pr_info("init\n");
	return 0;
}

static void __exit my_exit(void)
{
	proc_remove(entry);
	pr_info("exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
