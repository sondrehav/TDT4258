/*
 * This is a demo Linux kernel module.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

/*
 * template_init - function to insert this module into kernel space
 *
 * This is the first of two exported functions to handle inserting this
 * code into a running kernel
 *
 * Returns 0 if successfull, otherwise -1
 */

dev_t dev;

static struct file_operations gpad_fops = {
	.owner = 	THIS_MODULE;
	.read = 	gpad_read;
	.write = 	gpad_write;
	.open = 	gpad_open;
	.release = 	gpad_release;
};

struct cdev gpad_cdev;
struct class *cl;

static int __init template_init(void)
{
	printk("Hello World, here is your module speaking\n");
	alloc_chrdev_region(&dev, 0, 1, "GamepadDriver");
	cdev_init($gpad_cdev, &gpad_fops);
	int err = cdev_add(&gpad_cdev, dev, 1);
	if(err){
		printk(KERN_NOTICE "Error %d adding driver.", err);
		return = 1;
	}
	cl = class_create(THIS_MODULE, "GamepadDriver");
	device_create(cl, NULL, dev, NULL, "GamepadDriver");
	return 0;
}

static int gpad_open(struct inode *inode, struct file *filep){
	printk(KERN_NOTICE "Open driver!");
	/*struct gpad_dev *dev_;
	dev_ = container_of(inode->i_cdev, struct gpad_dev, cdev);
	filep->private_data = dev_;*/
}

static int gpad_release(struct inode *inode, struct file *filep){
	printk(KERN_NOTICE "Release driver!");
}

static ssize_t gpad_read(struct file *filep, char __user *buf, size_t count, loff_t *offsetp){
	printk(KERN_NOTICE "Read driver!");	
}

static ssize_t gpad_write(struct file *filep, char __user *buf, size_t count, loff_t *offsetp){
	printk(KERN_NOTICE "Write driver!");	
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

