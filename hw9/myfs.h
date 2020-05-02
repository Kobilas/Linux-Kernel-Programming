#include <linux/types.h>
#include <linux/fs.h>
#define MYFS_MAGIC 0x78902020
#define MYFS_SUPERBLOCK_BLOCK_NUMBER = 0
#define MYFS_ROOT_INODE_NUMBER = 1

static const unsigned long MYFS_MAGIC = 0X77889900

struct myfs_super_block {
	unsigned long msb_magic;
	unsigned long msb_inode_blocks;
	unsigned long msb_block_size;
	unsigned long msb_root_inode;
	unsigned long msb_inodes_in_block;
};
