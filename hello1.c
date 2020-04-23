#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Kobilas <mk666@njit.edu>");
MODULE_DESCRIPTION("CS680 Homework 2 - hello1.c");

static int __init hello1_init(void){
  printk(KERN_INFO "Matthew Kobilas: Loading Hello1 module - Hello World 1\n");
  return 0;
}

static void __exit hello1_exit(void){
  printk(KERN_INFO "Matthew Kobilas: Exiting Hello1 module - Hello World 1\n");
}

module_init(hello1_init);
module_exit(hello1_exit);

