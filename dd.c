#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#define DEV_MAJOR 89
#define DEV_MINOR 1
#define DEV_NAME "myDev"



/**
 * Attributes
 */

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Device Driver Demo");
MODULE_AUTHOR("Ciro S. Costa");

static char msg[100] = {0};
static short readPos = 0;
static int times = 0;

/**
 * Prototypes
 */

static int dev_open(struct inode *, struct file *);
static int dev_rls(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t *, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);


/**
 * Structure containing the callbacks
 */

static struct file_operations fops =
{
	.read = dev_read,
	.open = dev_open,
	.write = dev_write,
	.release = dev_rls
};

/**
 * Module stuff
 */


/**
 * Registers the device driver for a particular
 * major number.
 */
int init_module(void)
{
	int t = register_chrdev(DEV_MAJOR, DEV_NAME, &fops);

	if (t < 0)
		printk(KERN_ALERT "Device registration Failed");
	else
		printk(KERN_ALERT "Device Registered ..\n");

	return t;
}

void cleanup_module(void)
{
	unregister_chrdev(DEV_MAJOR, DEV_NAME);
}

/**
 * Called when an 'open' system call is done on
 * the device file.
 */
static int dev_open(struct inode * inod, struct file * fil)
{
	times++;
	printk(KERN_ALERT "Device Opened %d times\n", times);

	return 0;
}

/**
 * Called when 'read' system call is done on the
 * device file.
 */
static ssize_t dev_read(struct file * filp, char * buff, size_t * len, loff_t * off)
{
	short count = 0;
	while (len && (msg[readPos] != 0)) {
		put_user(msg[readPos], buff++);  /* copy byte from kernel space to user space */
		count++;
		len--;
		readPos++;
	}

	return count;
}

/**
 * Called when 'write' system call is done on
 * the device file.
 */
static ssize_t dev_write(struct file * fil, const char * buff, size_t len, loff_t * off)
{
	short ind = len - 1;
	short count = 0;

	memset(msg, 0, 100);
	readPos = 0;

	while (len > 0) {
		msg[count++] = buff[ind--];
		len--;
	}

	return count;
}

/**
 * Called when 'close' system call is done on
 * the device file.
 */
static int dev_rls(struct inode * inod, struct file * fil)
{
	printk(KERN_ALERT "Device Closed\n");

	return 0;
}

