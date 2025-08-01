#include <linux/module.h>
#include <linux/fs.h>

#define DRIVER_AUTHOR "huysn11b@gmail.com"
#define DRIVER_DESC   "Hello kernel module"


static int __init hello_init(void)
{
    pr_info("Hello world kernel module\n");
    return 0;
}

static void __exit hello_exit(void)
{
    pr_info("Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);  