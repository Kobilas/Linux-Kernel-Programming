#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matthew Kobilas <mk666@njit.edu>");
MODULE_DESCRIPTION("CS680 Homework 2 - hello3.c");

static char *myname = "default_name";
static char *street = "default_street";
static char *city = "default_city";
static int zipcode = 0;

module_param(myname, charp, 0);
MODULE_PARM_DESC(myname, "string to store name");
module_param(street, charp, 0);
MODULE_PARM_DESC(street, "string to store street address");
module_param(city, charp, 0);
MODULE_PARM_DESC(city, "string to store city");
module_param(zipcode, int, 0);
MODULE_PARM_DESC(zipcode, "int to store zipcode");

static int __init hello3_init(void){
  printk(KERN_INFO "Loading Hello 3: Hello World 3\n");
  printk(KERN_INFO "==============================\n");
  printk(KERN_INFO "Name : %s\n", myname);
  printk(KERN_INFO "Street : %s\n", street);
  printk(KERN_INFO "City: %s\n", city);
  printk(KERN_INFO "ZIP code: %d\n", zipcode);
  return 0;
}

static void __exit hello3_exit(void){
  printk(KERN_INFO "Matthew Kobilas: Exiting Hello3 module - Hello World 3\n");
}

module_init(hello3_init);
module_exit(hello3_exit);

