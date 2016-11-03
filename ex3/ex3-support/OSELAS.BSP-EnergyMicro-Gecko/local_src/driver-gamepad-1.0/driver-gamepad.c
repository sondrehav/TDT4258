/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>          // Required for the copy to user function
#include <linux/ioport.h>
#include <asm/io.h>

dev_t dev;

static int gpad_open(struct inode *inode, struct file *filep);
static int gpad_release(struct inode *inode, struct file *filep);
static ssize_t gpad_read(struct file *filep, char __user *buf, size_t count, loff_t *offsetp);
static ssize_t gpad_write(struct file *filep, char __user *buf, size_t count, loff_t *offsetp);

static struct file_operations gpad_fops = {
	.owner = 	THIS_MODULE,
	.read = 	gpad_read,
	.write = 	gpad_write,
	.open = 	gpad_open,
	.release = 	gpad_release
};

struct cdev gpad_cdev;
struct class *cl;


/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */


struct resource* res;

static int __init template_init(void)
{
	printk("Hello World, here is your module speaking\n");
	alloc_chrdev_region(&dev, 0, 1, "GamepadDriver");
	cdev_init(&gpad_cdev, &gpad_fops);
	int err = cdev_add(&gpad_cdev, dev, 1);
	if(err){
		printk(KERN_NOTICE "Error %d adding driver.", err);
		return -1;
	}
	cl = class_create(THIS_MODULE, "GamepadDriver");
	device_create(cl, NULL, dev, NULL, "GamepadDriver");
	printk("Initialization done.");

	res = request_mem_region(GPIO_PA_BASE, 0x24, "GamepadDriver");
	if(res == NULL) {
		printk(KERN_ERR "request_mem_region returned null.");
		return -1;
	}
	

	char* hello = ioremap_nocache(res->start, res->0x24);
	printk(KERN_NOTICE "%d", hello);

	return 0;
}

static int gpad_open(struct inode *inode, struct file *filep){
	printk(KERN_NOTICE "Open driver!\n");
	return 0;
}

static int gpad_release(struct inode *inode, struct file *filep){
	printk(KERN_NOTICE "Release driver!\n");
	return 0;
}

static char message[256] = {0};
static short size_of_message;

// Send string to user! When program calls fgets.
static ssize_t gpad_read(struct file *filep, char __user *buf, size_t count, loff_t *offsetp){
	int error_count = copy_to_user(buf, message, size_of_message +1);
	int i = 0;
	if(error_count == 0){
		printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
		return 256;
	} else {
		printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
		return -14; 
	}
}

// User sends to driver. When program calls fprintf.
static ssize_t gpad_write(struct file *filep, char __user *buf, size_t count, loff_t *offsetp){
	printk(KERN_NOTICE "Write driver!\n");
	printk(KERN_NOTICE "Driver: %s\n", buf);
	int i = 0;
	while(i<256) {
		message[i] = buf[i];
		if(buf[i] == '\0') {
			size_of_message = i;
			return size_of_message;
		}
		i++;
	}
	printk(KERN_INFO "Error: Could not find null-terminator.");
	return 0;
}


/*
 * template_cleanup - function to cleanup this module from kernel space
 *
 * This is the second of two exported functions to handle cleanup this
 * code from a running kernel
 */

static void __exit template_cleanup(void)
{
	cdev_del(&gpad_cdev);
	unregister_chrdev_region(dev, 1);
	printk("Short life for a small module...\n");
}

module_init(template_init);
module_exit(template_cleanup);

MODULE_DESCRIPTION("Small module, demo only, not very useful.");
MODULE_LICENSE("GPL");

