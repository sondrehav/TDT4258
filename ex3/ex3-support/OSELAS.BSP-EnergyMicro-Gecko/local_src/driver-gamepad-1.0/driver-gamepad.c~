/*
 * This is a demo Linux kernel module.
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <asm/uaccess.h>	// Required for the copy to user function
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/signal.h>
#include "efm32gg.h"

dev_t dev;

static int gpad_open (struct inode *inode, struct file *filep);
static int gpad_release (struct inode *inode, struct file *filep);
static ssize_t gpad_read (struct file *filep, char __user * buf, size_t count,
			  loff_t * offsetp);
static ssize_t gpad_write (struct file *filep, char __user * buf,
			   size_t count, loff_t * offsetp);
static int gpad_fasync (int fd, struct file *filep, int on);
irqreturn_t button_interrupt (int irq, void *dev_id, struct pt_regs *regs);

static struct file_operations gpad_fops = {
  .owner = THIS_MODULE,
  .read = gpad_read,
  .write = gpad_write,
  .open = gpad_open,
  .release = gpad_release,
  .fasync = gpad_fasync
};

struct cdev gpad_cdev;
struct class *cl;
struct fasync_struct *async_queue;
struct resource *res;

// Setting up module
static int __init
template_init (void)
{
  // Initialization
  alloc_chrdev_region (&dev, 0, 1, "GamepadDriver");
  cdev_init (&gpad_cdev, &gpad_fops);
  int err = cdev_add (&gpad_cdev, dev, 1);
  if (err)
    {
      printk (KERN_NOTICE "Error %d adding driver.", err);
      return -1;
    }
  cl = class_create (THIS_MODULE, "GamepadDriver");
  device_create (cl, NULL, dev, NULL, "GamepadDriver");
  printk ("Initialization done.");


  // Request memory regions
  res = request_mem_region (GPIO_PC_BASE, 0x24, "GamepadDriver");	// 0x24 = size of GPIO_PC adresses
  if (res == NULL)
    {
      printk (KERN_ERR "request_mem_region returned null.");
      return -1;
    }
  ioremap_nocache (res->start, 0x24);

  res = request_mem_region (GPIO_EXTIPSELL, 0x12, "GamepadDriver");	// 0x12 = size of memory from GPIO_EXTIPSELL to GPIO_IFC 
  if (res == NULL)
    {
      printk (KERN_ERR "request_mem_region returned null.");
      return -1;
    }
  ioremap_nocache (res->start, 0x12);

  // Setting up buttons
  iowrite32 (0x33333333, GPIO_PC_MODEL);
  iowrite32 (0xff, GPIO_PC_DOUT);

  // Setting up interrupt
  iowrite32 (0x22222222, GPIO_EXTIPSELL);
  iowrite32 (0xff, GPIO_EXTIFALL);
  iowrite32 (0xff, GPIO_EXTIRISE);
  iowrite32 (0xff, GPIO_IEN);
  iowrite32 (0xff, GPIO_IFC);
  request_irq (18, button_interrupt, 0, "GamepadDriver", NULL);

  return 0;
}

static uint32_t button_value;

irqreturn_t
button_interrupt (int irq, void *dev_id, struct pt_regs *regs)
{
  // Clear interrupt
  uint32_t gpio_if_value = ioread32 (GPIO_IF);
  iowrite32 (gpio_if_value, GPIO_IFC);
  //Send signal to interrupt user application
  if (async_queue)
    {
      kill_fasync (&async_queue, SIGIO, POLL_IN);
    }
  return IRQ_HANDLED;
}

// Open driverfile
static int
gpad_open (struct inode *inode, struct file *filep)
{
  printk (KERN_NOTICE "Open driver!\n");
  return 0;
}

// Close driverfile
static int
gpad_release (struct inode *inode, struct file *filep)
{
  printk (KERN_NOTICE "Release driver!\n");
  return 0;
}

// Send string to user! When program calls fgets.
static ssize_t
gpad_read (struct file *filep, char __user * buf, size_t count,
	   loff_t * offsetp)
{
  // Write buttonvalue so user can read value 
  button_value = ioread32 (GPIO_PC_DIN);
  int error_count = copy_to_user (buf, &button_value, 4);
  if (error_count == 0)
    {
      return 3;
    }
  else
    {
      printk (KERN_INFO "EBBChar: Failed to send %d characters to the user\n",
	      error_count);
      return -14;
    }

}

// User sends to driver. When program calls fprintf.
static ssize_t
gpad_write (struct file *filep, char __user * buf, size_t count,
	    loff_t * offsetp)
{
  // Not used
  return 0;
}

static int
gpad_fasync (int fd, struct file *filep, int on)
{
  return fasync_helper (fd, filep, on, &async_queue);
}

//template_cleanup - function to cleanup this module from kernel space
static void __exit
template_cleanup (void)
{
  free_irq (18, NULL);
  release_mem_region (GPIO_PC_BASE, 0x24);
  release_mem_region (GPIO_EXTIPSELL, 0x12);

  cdev_del (&gpad_cdev);
  unregister_chrdev_region (dev, 1);
  printk ("Short life for a small module...\n");
}

module_init (template_init);
module_exit (template_cleanup);

MODULE_DESCRIPTION ("Small module, demo only, not very useful.");
MODULE_LICENSE ("GPL");
