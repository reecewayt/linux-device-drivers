/*
 * e1000-driver.c is an implementation of a PCI Ethernet device driver. The device controls
 * the LED on the e1000 NIC card.
 *
 * 00:03.0 Ethernet controller: Intel Corporation 82540EM Gigabit Ethernet Controller
 *
 * Date: 4/17/2024
 * Author: Reece Wayt
 * Sources:
 * "Linux Device Driver Development (2nd Edition)" by John Madieu
 * 82583v-gbe-controller-datasheet.pdf
 * https://support.elmark.com.pl/advantech/pdf/aimb-212man.pdf
 */

#include "my-e1000-driver.h"

// Global device structure
struct my_device mydev;

// Initialize dev structure
static struct pci_driver e1000_driver = {
    .name = "my_e1000",
    .id_table = pci_ids,
    .probe = e1000_probe,
    .remove = e1000_remove,
};

int e1000_probe(struct pci_dev *pdev, const struct pci_device_id *id) {
    int err, bars;
    resource_size_t mmio_start, mmio_len;

    err = pci_enable_device(pdev);
    if (err) {
        printk(KERN_ERR "PCI enable device failed...\n");
        return err;
    }

    // Request the memory region
    bars = pci_select_bars(pdev, IORESOURCE_MEM);
    err = pci_request_selected_regions(pdev, bars, DEVICE_NAME);
    if (err) {
        printk(KERN_ERR "PCI request select regions failed...\n");
        goto err_pci_reg;
    }

    mmio_start = pci_resource_start(pdev, 0);
    mmio_len = pci_resource_len(pdev, 0);

    mydev.hw_addr = ioremap(mmio_start, mmio_len);
    if (!mydev.hw_addr) {
        printk(KERN_ERR "PCI ioremap failed...\n");
        goto err_pci_ioremap;
    }

    printk(KERN_INFO "e1000: Device %s probed successfully\n", pci_name(pdev));
    return 0;

err_pci_ioremap:
    pci_release_selected_regions(pdev, bars);
    err = -EIO;
err_pci_reg:
    pci_disable_device(pdev);
    return err;
}

void e1000_remove(struct pci_dev *pdev) {
    // Unmap the I/O region
    iounmap(mydev.hw_addr);

    // Release PCI regions
    pci_release_selected_regions(pdev, pci_select_bars(pdev, IORESOURCE_MEM));

    // Disable PCI device
    pci_disable_device(pdev);

    printk(KERN_INFO "e1000: Device %s removed successfully\n", pci_name(pdev));
}

ssize_t e1000_read(struct file *filp, char __user *buffer, size_t len, loff_t *offset) {
    u32 led_status;

    led_status = ioread32(mydev.hw_addr + E1000_LEDCTL);
    printk(KERN_INFO "LED status: %u\n", led_status);

    if (len < sizeof(led_status)) {
        return -EINVAL;
    }
    if (copy_to_user(buffer, &led_status, sizeof(led_status))) {
        return -EFAULT;
    }

    return sizeof(led_status);
}


ssize_t e1000_write(struct file *filp, const char __user *buffer, size_t len, loff_t *offset) {
    u32 led_command;

    if(len < sizeof(led_command)){
        printk(KERN_ERR "Buffer size is less than 4 bytes\n");
        return -EFAULT; 
    }
    if (copy_from_user(&led_command, buffer, sizeof(led_command))){
        printk(KERN_ERR "Failed to copy from user...\n");
        return -EFAULT; 
    }
    mutex_lock(&mydev.lock);
    iowrite32(led_command, mydev.hw_addr + E1000_LEDCTL);
    mutex_unlock(&mydev.lock);

    printk(KERN_INFO "Wrote LED ctrl success, %x\n", led_command);

    return sizeof(led_command); 
}

int e1000_open(struct inode *inode, struct file *filp) {
    filp->private_data = &mydev;
    return 0;
}

// e1000_release method simply undoes the e1000_open() method
int e1000_release(struct inode *inode, struct file *filp) {
    filp->private_data = NULL;
    return 0;
}

// Callback methods
static struct file_operations my_fops = {
    .open = e1000_open,
    .read = e1000_read,
    .write = e1000_write,
    .release = e1000_release,
};

static int __init e1000_init(void) {
    int error;

    // Initialize mutex
    mutex_init(&mydev.lock);

    printk(KERN_INFO "PCI Ethernet driver initializing...\n");

    // Register character device
    error = alloc_chrdev_region(&mydev.dev_num, 0, 1, DEVICE_NAME);
    if(error < 0) {
        printk(KERN_ERR "Can't get device number for %d\n", error);
        return error;
    }

    // Create device class
    mydev.class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(mydev.class)) {
        error = PTR_ERR(mydev.class);
        printk(KERN_ERR "Device class failed: %d\n", error);
        goto err_class;
    }
     //Initialize cdev struct and file_ops struct
    cdev_init(&mydev.cdev, &my_fops);
    mydev.cdev.owner = THIS_MODULE;

    //Register cdev structure to kernel
    error = cdev_add(&mydev.cdev, mydev.dev_num, 1); 
    if(error < 0){
        printk(KERN_ERR "Failed to add cdev structure for device: %d\n", error);
        goto err_cdev_add;
    }

    //Creating device file for use in /dev directory
    mydev.device = device_create(mydev.class, NULL, mydev.dev_num, NULL, DEVICE_NAME);
    
    if(IS_ERR(mydev.device)){
        error = PTR_ERR(mydev.device);
        printk(KERN_ERR "Failed creating device file in /dev: %d\n", error);
        goto err_device_create;
    }

    // Register PCI driver
    error = pci_register_driver(&e1000_driver);
    if (error < 0) {
        printk(KERN_ERR "Failed to register PCI driver: %d\n", error);
        return error;
    }
    // Init successful
    printk(KERN_INFO "PCI Ethernet driver is loaded.\n");

    return 0;


//Cleanup labels upon error
err_device_create:
    cdev_del(&mydev.cdev);
err_cdev_add:
    class_destroy(mydev.class);
err_class: 
    unregister_chrdev_region(mydev.dev_num, 1);
    return error;

}

static void __exit e1000_exit(void) {
    pci_unregister_driver(&e1000_driver);
    device_destroy(mydev.class, mydev.dev_num);
    cdev_del(&mydev.cdev);
    class_destroy(mydev.class);
    unregister_chrdev_region(mydev.dev_num, 1);
    mutex_destroy(&mydev.lock);
    printk(KERN_INFO "Unloaded %s\n", DEVICE_NAME);
}

module_init(e1000_init);
module_exit(e1000_exit);

MODULE_AUTHOR("Reece Wayt <reecwayt@pdx.edu>");
MODULE_DESCRIPTION("PCI Ethernet device driver for assignment 4");
MODULE_LICENSE("GPL");