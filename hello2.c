#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Kobilas <mk666@njit.edu>");
MODULE_DESCRIPTION("CS680 Homework 2 - hello2.c");

static int hello2_data __initdata = 2;

static int __init hello2_init(void){
  printk(KERN_INFO "Matthew Kobilas: Loading Hello2 module - Hello World %d\n", hello2_data);
  return 0;
}

static void __exit hello2_exit(void){
  printk(KERN_INFO "Matthew Kobilas: Exiting Hello2 module - Goodbye World 2");
}

module_init(hello2_init);
module_exit(hello2_exit);
