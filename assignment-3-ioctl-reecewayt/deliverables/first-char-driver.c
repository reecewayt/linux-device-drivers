/*
 * first-char-driver.c is a simple implementation of a character driver. The device does nothing useful but outlines
 * the general steps to creating a device node and class and using internal data and methods to interact with the driver. Unlike
 * assingment 2, this program implements ioctl commands. 
 * 
 * Date: 4/17/2024
 * Author: Reece Wayt
 * Sources: 
 * "Linux Device Driver Development (2nd Edition)" by John Madieu
 * https://github.com/PacktPublishing/Linux-Device-Driver-Development-Second-Edition/blob/main/Chapter04/dummy-char.c
 * For editing driver permission: 
 * https://www.kernel.org/doc/html/v4.14/driver-api/infrastructure.html
 * https://forums.raspberrypi.com/viewtopic.php?t=205882
 * https://stackoverflow.com/questions/11846594/how-can-i-programmatically-set-permissions-on-my-char-device
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <uapi/asm-generic/ioctl.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "first-char-driver"
#define CLASS_NAME "first-char-class"

//ioctl command number gen, defined in the source code in ioctl.h
#define MYCHAR_IOC_MAGIC 'z'
#define IOC_NR_MAX 14   //shouldn't have more than 14 command numbers

// _IOR & _IOW macro constructs a command number for reading & writing data from the device.
#define MY_IOC_R_CMD _IOR(MYCHAR_IOC_MAGIC, 1, int)
#define MY_IOC_W_CMD _IOW(MYCHAR_IOC_MAGIC, 2, int)

//func prototypes
int fchar_open(struct inode *inode, struct file *filp);
ssize_t fchar_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset);
ssize_t fchar_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
int fchar_release(struct inode *inode, struct file *filp);
//ioctl command set
long fchar_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);




//used to pass param value during device instantiation
static int cmdl_val = 0;
module_param(cmdl_val, int, 0660); //cmdl_val is stored in /sys/module/first_char_driver/parameters

struct my_first_cdev {
    unsigned int major;     //stores device major number
    unsigned int minor;     //stores device minor number
    struct mutex lock;      // lock for writing to device
    int syscall_val;        // System call variable per assignment instructions
    struct class *class;    //stores class for this device
    struct cdev cdev;       //stores cdev struct object
    struct device *device;  //stores device struct object
    dev_t dev_num;          // Holds device number (minor and major)
};

//initialize dev structure 
static struct my_first_cdev dev_data;  

/*
fchar_open- ()Open callback method for this device driver. 

Parameters: 
    - struct inode *inode: metadata about open file
    - struct file *filp: struct file pointer that corresponds to file descriptor of the 
    opening process

Returns: 0 on success and negative error on failure
*/
int fchar_open(struct inode *inode, struct file *filp) {
    struct my_first_cdev *dev_data = NULL;
    printk(KERN_INFO "Attempting to open device %s", DEVICE_NAME);
    dev_data = container_of(inode->i_cdev, struct my_first_cdev, cdev);
    
    if(!dev_data){

        printk(KERN_ERR "Device data returned NULL from container_of() method...\n");
        return -ENODEV; //no device
    }
    
    //set private data for struct file for success file ops; essentially stores
    //device specific structure inside the calling process's file descriptor
    filp->private_data = dev_data;

    printk(KERN_INFO "Device opened: %s... SUCCESS!\n", DEVICE_NAME);
    return 0;
}

ssize_t fchar_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset) {
    ssize_t retval; 
    int cur_val; 
    struct my_first_cdev *dev_data = NULL;
    dev_data = filp->private_data;
    printk(KERN_INFO "Attempting to read device %s", DEVICE_NAME);
    if (!dev_data) {
        printk(KERN_ERR "No device data found on read.\n");
        return -ENODEV;
    }
    cur_val = dev_data->syscall_val;

    retval = copy_to_user(buffer, &cur_val, len);
    if(retval != 0){
        printk(KERN_ERR "Failed to read %zu bytes from device...\n", retval);
        return -EFAULT; 
    }

    return (len - retval);
}

ssize_t fchar_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset) {
    //return value for number of bytes written
    ssize_t retval; 
    int new_val;
    struct my_first_cdev *dev_data = filp->private_data;
    printk(KERN_INFO "Attempting to write device %s", DEVICE_NAME);
    if (!dev_data) {
        printk(KERN_ERR "No device data found on write.\n");
        return -ENODEV;
    }
    //copy value from user space and update device struct in kernel
    mutex_lock(&dev_data->lock);
    retval = copy_from_user(&new_val, buffer, len);
    if(retval != 0){
        printk(KERN_ERR "Failed to copy %zu bytes from user...\n", retval);
        mutex_unlock(&dev_data->lock);
        return -EFAULT; 
    }
    dev_data->syscall_val = new_val;
    printk(KERN_INFO "Wrote %d to device driver...SUCCESS!\n", dev_data->syscall_val);
    mutex_unlock(&dev_data->lock);

    return (len - retval); //returns number of bytes written
}

//fchar_release method simply undoes the fchar_open() method
int fchar_release(struct inode *inode, struct file *filp) {
    struct my_first_cdev *dev_data = filp->private_data;
    printk(KERN_INFO "Attempting to close device %s", DEVICE_NAME);
    if (!dev_data) {
        printk(KERN_ERR "No device data found on release.\n");
        return -ENODEV;
    }

    filp->private_data = NULL; //reset private_data pointer

    return 0;
}

long fchar_ioctl(struct file *filp, unsigned int cmd, unsigned long arg) {
    struct my_first_cdev *dev_data = filp->private_data;
    printk(KERN_INFO "Attempting ioctl command on %s", DEVICE_NAME);
    if (!dev_data) {
        printk(KERN_ERR "No device data found on ioctl call.\n");
        return -ENODEV;
    }
    switch(cmd) {
        case MY_IOC_R_CMD: // Read from device to user space
            if(put_user(dev_data->syscall_val, (int __user *)arg)){
                printk(KERN_ERR "Could not read from device using ioctl %s", DEVICE_NAME);
                return -EACCES;
            }
            break;
            

        case MY_IOC_W_CMD: // Write to device from user space
            // acquire mutex lock on write
            mutex_lock(&dev_data->lock);
            if(get_user(dev_data->syscall_val, (int __user *)arg)){
                mutex_unlock(&dev_data->lock);
                printk(KERN_ERR "Could not write to device using ioctl %s", DEVICE_NAME);
                return -EACCES; 
            }
            mutex_unlock(&dev_data->lock);
            break;

        default:
            return -ENOTTY; //command not supported  
    }
    return 0; //Success
}

//Callback methods 
static struct file_operations my_fops = { 
    .open = fchar_open,
    .read = fchar_read,
    .write = fchar_write, 
    .release = fchar_release,
    .unlocked_ioctl = fchar_ioctl,
};

//Callback method to assert driver permission levels
static char *new_devnode(struct device *device, umode_t *mode){
    //safe guard to check if NULL, mode should be init after class is created
    if(mode){ 
        *mode = 0666;
    }
    return NULL; 
}

static int __init first_char_init(void){
    long int error; 
    //initialize syscall per assignment instructions
    dev_data.syscall_val = cmdl_val;
    //initialize mutex
    mutex_init(&dev_data.lock);
    
    printk(KERN_INFO "First character driver initializing...\n");
    
    //1. Setting up the char device
    //Alloc to get major and minor numbers for one device
    error = alloc_chrdev_region(&dev_data.dev_num, 0, 1, DEVICE_NAME); 
    if(error < 0) {
        printk(KERN_ERR "Can't get device number for %ld\n", error);
        return error; //return error code
    }
    dev_data.major = MAJOR(dev_data.dev_num);
    dev_data.minor = MINOR(dev_data.dev_num);

    //Create device class, visible in /sys/class
    dev_data.class = class_create(THIS_MODULE, CLASS_NAME);
    if(IS_ERR(dev_data.class)){
        error = PTR_ERR(dev_data.class);
        printk(KERN_ERR "Can't create device class for %ld\n", error);
        goto err_class; 
    }
    //Modify device file permission to allow user access
    dev_data.class->devnode = new_devnode;

    //Initialize cdev struct and file_ops struct
    cdev_init(&dev_data.cdev, &my_fops);
    dev_data.cdev.owner = THIS_MODULE;

    //Register cdev structure to kernel
    error = cdev_add(&dev_data.cdev, dev_data.dev_num, 1); 
    if(error < 0){
        printk(KERN_ERR "Failed to add cdev structure for device: %ld\n", error);
        goto err_cdev_add;
    }

    //Creating device file for use in /dev directory
    dev_data.device = device_create(dev_data.class, NULL, dev_data.dev_num, NULL, DEVICE_NAME);
    
    if(IS_ERR(dev_data.device)){
        error = PTR_ERR(dev_data.device);
        printk(KERN_ERR "Failed creating device file in /dev: %ld\n", error);
        goto err_device_create;
    }
    //init successful
    return 0;

//Cleanup labels upon error
err_device_create:
    cdev_del(&dev_data.cdev);
err_cdev_add:
    class_destroy(dev_data.class);
err_class: 
    unregister_chrdev_region(dev_data.dev_num, 1);
    return error;
}

static void __exit first_char_exit(void) {
    device_destroy(dev_data.class, dev_data.dev_num);
    cdev_del(&dev_data.cdev);
    class_destroy(dev_data.class);
    mutex_destroy(&dev_data.lock);
    unregister_chrdev_region(dev_data.dev_num, 1);
    printk(KERN_INFO "Unloaded %s\n", DEVICE_NAME);
}

module_init(first_char_init);
module_exit(first_char_exit);



MODULE_AUTHOR("Reece Wayt <reecwayt@pdx.edu");
MODULE_DESCRIPTION("First character device driver for assignment 2");
MODULE_LICENSE("GPL");
