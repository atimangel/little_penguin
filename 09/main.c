#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/proc_fs.h>
#include <linux/nsproxy.h>
#include <linux/rbtree.h>
#include <../fs/mount.h>

MODULE_LICENSE("GPL");

static struct proc_dir_entry *mymounts;

static void show_mount_info(struct seq_file *m, struct mount *mnt)
{

		if (!strcmp(mnt->mnt_devname, "rootfs"))
			return;

		struct path mnt_path =  {
			dentry : mnt->mnt.mnt_root,
			mnt : &mnt->mnt
		};

		struct super_block *sb = mnt_path.dentry->d_sb;

		if (sb->s_op->show_devname)
			sb->s_op->show_devname(m, mnt_path.dentry);
		else
			seq_puts(m, mnt->mnt_devname ? mnt->mnt_devname : "none");

		seq_putc(m, '\t');
		seq_putc(m, '\t');
		seq_path(m, &mnt_path, "");
		seq_putc(m, '\n');
}

static int procfs_show(struct seq_file *m, void *v)
{
	struct rb_node *node;
	struct mount *mnt;

	for (node = rb_first(&current->nsproxy->mnt_ns->mounts); node; node = rb_next(node)) {
		mnt = rb_entry(node, struct mount, mnt_node);
		show_mount_info(m, mnt);
	}

	return 0;
}

static int procfs_open(struct inode *inode, struct file *file)
{
	return single_open(file, procfs_show, NULL);
}

static const struct proc_ops proc_fops= {
	.proc_open = procfs_open,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = single_release,
};

static int __init my_init(void)
{
	mymounts = proc_create("mymounts", 0, NULL, &proc_fops);
	if (!mymounts) {
		pr_err("failed to create /proc/mymounts file\n");
		return -ENOMEM;
	}
	pr_info("init\n");
	return 0;
}

static void __exit my_exit(void)
{
	remove_proc_entry("mymounts", NULL);
	pr_info("exit\n");
}

module_init(my_init);
module_exit(my_exit);

