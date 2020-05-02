#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>

#include "myfs.h"

static struct super_operations myfs_super_operations = {
	// not sure what statfs is supposed to do, so commenting out for now
	//.statfs		= simple_statfs,
	.alloc_inode	= ext4_alloc_inode,
};

static struct inode_operations myfs_inode_operations = {
	.lookup		= simple_lookup,
	.link		= simple_link,
	.mkdir		= simple_mkdir,
};

static struct file_operations myfs_file_operations = {
	.open		= myfs_open,
	.read		= myfs_read_file,
	.write		= myfs_write_file,
};

static int myfs_fill_super(struct super_block *sb, void *data, int silent) {
	printk(KERN_INFO "Matthew Kobilas fs: myfs.c:myfs_fill_super()\n");
	
	struct buffer_head *bh;
	struct inode *root;
	struct myfs_super_block *msb;
	bh = sb_bread(sb, 0);
	msb = (struct myfs_super_block *)bh->b_data;
	
	if (!msb) {
		return -1;
	}
	
	sb->s_magic = MYFS_MAGIC;
	sb->s_fs_info = msb;
	sb->s_op = &myfs_super_operations;
	
	root = new_inode(sb);
	inode->i_ino = MYFS_ROOT_INODE_NUMBER;
	inode_init_owner(inode, NULL, S_IFIDR);
	inode->i_sb = sb;
	inode->i_op = &myfs_inode_operations;
	inode->i_fop = &myfs_file_operations;
	sb->s_root = d_make_root(inode);
	
	return 0;
}

static struct dentry *myfs_mount(struct file_system_type *fs_type, int flags, const char *dev_name, void *data) {
	printk(KERN_INFO "Matthew Kobilas fs: myfs.c:myfs_mount()\n");
	struct dentry *entry = mount_bdev(fs_type, flags, dev_name, data, myfs_fill_super);
	if (IS_ERR(entry)){
		printk(KERN_ERR "Matthew Kobilas fs: myfs mounting failed\n");
	} else {
		printk(KERN_INFO "Matthew Kobilas fs: myfs mounted\n");
	}
	return entry;
}

static void myfs_kill_superblock(struct super_block *sb) {
	printk(KERN_INFO "Matthew Kobilas fs: myfs.c:myfs_kill_superblock()\n");
	kill_block_super(sb);
}

static struct file_system_type myfs_type = {
	.owner		= THIS_MODULE,
	.name		= "myfs",
	.mount		= myfs_mount,
	.kill_sb	= myfs_kill_superblock,
	.fs_flags	= FS_REQUIRES_DEV,
};

static int __init myfs_init_fs(void) {
	printk(KERN_INFO "Matthew Kobilas fs: myfs.c:myfs_init_fs()\n");
	return 0;
}

static void __exit myfs_exit_fs(void) {
	printk(KERN_INFO "Matthew Kobilas fs: myfs.c:myfs_exit_fs()");
}

module_init(myfs_init_fs);
module_exit(myfs_exit_fs);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Kobilas <mattkobi789@gmail.com>");
MODULE_DESCRIPTION("CS680 Assignment 9 - mini filesystem, myfs.c");
