#include <linux/module.h>
#include <linux/fs.h>      
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>

#define DRIVER_AUTHOR "huysn11b@gmail.com"
#define DRIVER_DESC   "EXAMPLE: Control led on/off with GPIO using kernel module"

#define NPAGES   1
#define BUFSIZE  16U

#define GPIO_NUMBER_27 27
#define LOW  0
#define HIGH 1


struct m_foo_dev {
    int size;
    char *kmalloc_ptr;
    dev_t dev_num;
    struct class *m_class;
    struct cdev m_cdev;
} mdev;

// Function Prototypes
static int      __init gpio_kernel_module_init(void);
static void     __exit gpio_kernel_module_exit(void);
static int      m_open(struct inode *inode, struct file *file);
static int      m_release(struct inode *inode, struct file *file);
static ssize_t  m_read(struct file *filp, char __user *user_buf, size_t size,loff_t *offset);
static ssize_t  m_write(struct file *filp, const char *user_buf, size_t size, loff_t *offset);

static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = m_read,
    .write      = m_write,
    .open       = m_open,
    .release    = m_release,
};

// This function will be called when we open the Device file
static int m_open(struct inode *inode, struct file *file)
{
    pr_info("System call open() called...!!!\n");

    return 0;
}

// This function will be called when we close the Device file
static int m_release(struct inode *inode, struct file *file)
{
    pr_info("System call close() called...!!!\n");

    return 0;
}

/* This function will be called when we read the Device file */
static ssize_t m_read(struct file *filp, char __user *user_buffer, size_t size, loff_t *offset)
{
    size_t to_read;

    pr_info("System call read() called...!!!\n");

    // Check size doesn't exceed our mapped area size
    to_read = (size > mdev.size - *offset) ? (mdev.size - *offset) : size;

	if (copy_to_user(user_buffer, mdev.kmalloc_ptr + *offset, to_read))
		return -EFAULT;

    *offset += to_read;

	return to_read;
}

/* This function will be called when we write the Device file */
static ssize_t m_write(struct file *filp, const char __user *user_buffer, size_t size, loff_t *offset)
{
    size_t to_write;
    char cmd_buf[BUFSIZE] = {0};

    pr_info("System call write() called...!!!\n");

    // Check size doesn't exceed our mapped area size
	to_write = (size + *offset > NPAGES * PAGE_SIZE) ? (NPAGES * PAGE_SIZE - *offset) : size;

	memset(mdev.kmalloc_ptr, 0, NPAGES * PAGE_SIZE);
	if (copy_from_user(mdev.kmalloc_ptr + *offset, user_buffer, to_write) != 0)
		return -EFAULT;

    strncpy(cmd_buf, mdev.kmalloc_ptr + *offset, sizeof(cmd_buf) - 1);

    pr_info("Data from user: %s", cmd_buf);

    if (0 == strncmp(cmd_buf, "on", 2)) {
        gpio_set_value(GPIO_NUMBER_27, HIGH);
        pr_info("GPIO27 set to HIGH, status: %d!\n", gpio_get_value(GPIO_NUMBER_27));
    } else if (0 == strncmp(cmd_buf, "off", 3)) {
        gpio_set_value(GPIO_NUMBER_27, LOW);
        pr_info("GPIO27 set to LOW, status: %d!\n", gpio_get_value(GPIO_NUMBER_27));
    } else {
        pr_err("Invalid command: %s\r\n", cmd_buf);
        return -EINVAL;
    }

    *offset += to_write;
    mdev.size = *offset;

	return to_write;
}

static int __init gpio_kernel_module_init(void)
{
    int ret;

    // Allocating device number (cat /pro/devices)
    if (alloc_chrdev_region(&mdev.dev_num, 0, 1, "m_cdev") < 0) {
	    pr_err("Failed to alloc chrdev region\n");
	    return -1;
    }
    pr_info("Major = %d Minor = %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));

    cdev_init(&mdev.m_cdev, &fops);

    if ((cdev_add(&mdev.m_cdev, mdev.dev_num, 1)) < 0) {
        pr_err("Cannot add the device to the system\n");
        goto rm_device_numb;
    }

    if ((mdev.m_class = class_create(THIS_MODULE, "m_gpio_class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_numb;
    }

    if ((device_create(mdev.m_class, NULL, mdev.dev_num, NULL, "m_gpio_device")) == NULL) {
        pr_err("Cannot create my device\n");
        goto rm_class;
    }

    if((mdev.kmalloc_ptr = kmalloc(NPAGES * PAGE_SIZE, GFP_KERNEL)) == 0){
        pr_err("Cannot allocate memory in kernel\n");
        goto rm_device;
    }

    ret = gpio_request(GPIO_NUMBER_27, "gpio_27");
    if (ret) {
        pr_err("gpio_request failed\n");
        return ret;
    }

    ret = gpio_direction_output(GPIO_NUMBER_27, LOW);
    if (ret) {
        pr_err("gpio_direction_output failed\n");
        gpio_free(GPIO_NUMBER_27);
        return ret;
    }
    pr_info("GPIO27 configured as output\r\n");

    return 0;

rm_device:
    device_destroy(mdev.m_class, mdev.dev_num);
rm_class:
    class_destroy(mdev.m_class);
rm_device_numb:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static void __exit gpio_kernel_module_exit(void)
{
    gpio_set_value(GPIO_NUMBER_27, LOW);
    gpio_free(GPIO_NUMBER_27);
    pr_info("GPIO27 set to LOW and released\r\n");

    kfree(mdev.kmalloc_ptr);
    device_destroy(mdev.m_class, mdev.dev_num);
    class_destroy(mdev.m_class);
    cdev_del(&mdev.m_cdev);
    unregister_chrdev_region(mdev.dev_num, 1);

    pr_info("Goodbye kernel\r\n");;
}

module_init(gpio_kernel_module_init);
module_exit(gpio_kernel_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);  