#include <linux/module.h>  /* Thu vien nay dinh nghia cac macro nhu module_init va module_exit */
#include <linux/fs.h>      /* Thu vien nay dinh nghia cac ham allocate major & minor number */

#define DRIVER_AUTHOR "huysn11b@gmail.com"
#define DRIVER_DESC   "Character device driver example"

struct m_chdev {
    dev_t dev_num;
} m_dev;

static int __init chdev_init(void)
{   
    /* 1.1 Dynamic allocating device number (cat /proc/devices) */
    if (alloc_chrdev_region(&m_dev.dev_num, 0, 1, "dev_num") < 0) {
	    pr_err("Failed to alloc chrdev region\n");
	    return -1;
    }

    /* 1.2 Static allocating device number (cat /proc/devices) */
    /* dev_t dev = MKDEV(173, 0); */
    // (register_chrdev_region(&mdev.dev_num, 1, "m-cdev")

    pr_info("Hello kernel\r\n");
    pr_info("Major = %d Minor = %d\r\n", MAJOR(m_dev.dev_num), MINOR(m_dev.dev_num));

    return 0;
}

static void __exit chdev_exit(void)
{
    /* 1. Unallocating device number */
    unregister_chrdev_region(m_dev.dev_num, 1);
    pr_info("Goodbye kernel\r\n");
}

module_init(chdev_init);
module_exit(chdev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);  