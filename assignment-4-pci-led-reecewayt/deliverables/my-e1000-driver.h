#ifndef MY_E1000_DRIVER_H
#define MY_E1000_DRIVER_H

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <linux/pci.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/err.h>
#include <linux/fs.h>
#include <linux/mutex.h>
#include <linux/kdev_t.h>
#include <linux/moduleparam.h>
#include <uapi/asm-generic/ioctl.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "my-e1000"
#define CLASS_NAME "my-e1000-class"

// e1000 ethernet device macros
#define E1000_VENDOR_ID 0x8086
#define E1000_DEVICE_ID 0x100e

// Giga Ethernet Control Registers
/* LED Control Reg Break Down, pg. 304 of Intel Dev Manual
    -------------------------------------
    |31 - 24 | 23 - 16 | 15 - 8 | 7 - 0 |
    | LED 3  | LED 2   | LED 1  | LED 0 |
    ------------------------------------- 
*/
#define E1000_LEDCTL                0x00E00 
#define E1000_LEDCTL_LED0_SHIFT		0
#define E1000_LEDCTL_LED1_SHIFT		8
#define E1000_LEDCTL_LED2_SHIFT		16
#define E1000_LEDCTL_LED3_SHIFT		24
//mode bits are [3:0]
#define E1000_LEDCTL_MODE_MASK		0x0F

#define E1000_LEDCTL_MODE_LED_ON	0xE //1110b led on
#define E1000_LEDCTL_MODE_LED_OFF	0xF //1111b led off



// Register PCI devices for this driver
static struct pci_device_id pci_ids[] = {
    { PCI_DEVICE(E1000_VENDOR_ID, E1000_DEVICE_ID), },
    { 0, }
};
MODULE_DEVICE_TABLE(pci, pci_ids);

// Function prototypes
int e1000_open(struct inode *inode, struct file *filp);
ssize_t e1000_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset);
ssize_t e1000_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset);
int e1000_release(struct inode *inode, struct file *filp);
int e1000_probe(struct pci_dev *pdev, const struct pci_device_id *id);
void e1000_remove(struct pci_dev *pdev);

// Device structure
struct my_device {
    struct cdev cdev;       // Character device structure
    dev_t dev_num;          // Device number
    struct class *class;    //stores class for this device
    struct device *device;  //stores device struct object
    struct mutex lock;      // Lock for writing to device
    struct pci_dev *pdev;
    void __iomem *hw_addr;  // Pointer to hw base address
};

extern struct my_device mydev;

#endif // MY_E1000_DRIVER_H