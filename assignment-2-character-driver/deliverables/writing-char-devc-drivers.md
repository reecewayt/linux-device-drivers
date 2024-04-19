# Writing Character Device Drivers
These are general notes from lecture and the textbook *Linux Device Driver Development, 2nd Edition* by John Madieu 
to serve as a general summary to write this type of device driver. 


### Summary of Steps to Creating a Character Device Drivers:

- Get major and Minor Numbers
    - Registered under `/proc/devices`
- Declare and register module parameters
    - Can be seen under `/sys/modules/<name>/parameters`
- Create device files
    - User space access
- Kernel only populates the device class and device info
    - create device class (class_create)- `/sys`
    - populate with major and minor number (device_create)
- User space needs to interpret and take action
    - udev daemon (or mount) creates `/dev` entry
- `cdev_init` to associate file operations with the device
- `cdev_add` to add the device to kernel device list

# Char device data structure introduction

- In linux systems character devices are represented as instances of `struct cdev` , declared in `include/linux/cdev.h`
    - Some points on this structure:
        - `struct module *owner = THIS MODULE` should be set to this macro
        - `const struct file_operations *ops` this is the operations associated with this character device
        - `dev_t dev` is the character device identifier (i.e. min and max numbers)

### Device file operations → `cdev->ops`

- This element points to the file operations supported by a given device
- Each operation is the target of a system call that gets redirected in the kernel to its file operation counterpart in `cdev->ops.struct file_operations`
- Full code of `file_operations` is in `include/linux/fx.h`

### File Representation in the Kernel

- Within the file operation structure at least one parameter for any function call is always of type `inode *` or `file *` . Inode refers to file on disk, and the `struct file*` refers to the file descriptor within a process (i.e. an open file).
- Within the `inode` structure, it’s important to set the `i_cdev` element when the file is a character device.

# Creating a device node

- creating a “node” makes a device visible to users and allows users to interact with the underlying device.
- There are several steps to make a node which are outlined below

### Device Identification

- This is a 32 bit unsigned number in elements of `dev_t`
    - **major -** is first 12 bits
    - **minor -** is the remaining 20 bits
- Source code on this is located in `include/linux/kdev_t.h`

**Registering a device:** 

For this assignment we are using the dynamic allocation method: `int alloc_chrdev_region(dev_t *dev, unsigned int firstminor, unsigned int count, char *name)`
- method returns 0 on success, or a negative error code on failure
- `dev` is the only output parameter. 

**Initializing and registering char devcs**
- Once device identifier is made- `dev_t`, you must initialize it to the system with `cdev_init()` and `cdev_add()`
- To remove from the system use `cdev_del()`

At this point the device is part of the system but not present in `/dev`
- Use `device_create()` for node/file to be created

# Implementing file operations
By implementing file ops we are enhancing the device functionality and exposing methods to the user space via system calls. 

### Exchanging Data Between Kernel and User Spaces
There are two principle function that allow this 
```c
// User -> Kernel
unsigned long copy_from_user(void *to, const void __user *from, unsigned long n);
// Kernel -> User
unsigned long copy_to_user(void __user *to, void *from, unsigned long n); 
```
- Note: `__user` points to the user space which is considered (untrusted memory) 
- Returns 0 upon success

### Implementing the `open` file operation
- This method is used for device and data structure initialization <br>
`int (*open) (struct inode *inode, struct file *filp)`
- Returns 0 on success, or negative error code on failure

**A side note on `container_of` method**
Here's the macro in the source code: 
```c
#define container_of(ptr, type, member) ({          \
    const typeof(((type *)0)->member) *__mptr = (ptr); \
    (type *)((char *)__mptr - offsetof(type, member)); })
```
- ptr: This is a pointer to the member within the container structure.
- type: This is the type of the container structure that includes the member.
- member: This is the name of the member within the container structure to which ptr points.

In my device driver code the `open()` method has this code: 
```c
struct my_first_cdev *dev_data;

dev_data = container_of(inode->i_cdev, struct my_first_cdev, cdev);
```
- inode->i_cdev: A pointer to cdev which is embedded in the my_first_cdev structure. Importantly this `inode` structure is passed to the `fchar_open()`  
method by the VFS when a `process` tries to open the device file.
    - inode review: (index node) is a data structure that stores information about a file or directory. Remember everything in linux is represented as such. The data  
    stored is metadata such as: permissions, owner, time stamps, file size, pointers to data blocks, file type. 
    - VFS (virtual file system): abstraction layer in the linux kernel that provides a common interface for file system operations. 

Here's how this works in context of Device Driver Development: 
- Device Files: Devices in Linux are accessed through special files located in /dev. These device files are associated with inodes, just like regular files. When a user program opens a device file, the VFS invokes the corresponding file operation provided by the device driver.
- File Operations Structure: Device drivers define a struct `file_operations` that includes pointers to functions such as open, read, write, and release. These functions handle the interactions with the device files.
- Using Inodes in Device Drivers: When a device file is opened, the device driver's open function is called with pointers to the inode (representing the device file's inode) and file (representing an open file description). The inode contains a pointer to the cdev structure (i_cdev) of the device, which is used to access the device-specific data or operations. 
- Again, the `container_of()` method is used to get the address of the custom device driver structure which should encapsulate all metadata for this device.

### Implementing the `release` file operation
- This is used for closing a device and you must undo everything done in the open operation <br>
`int (*release) (struct inode *inode, struct file *filp)`

### Implementing the `write` file operation
- Used to write data to a device <br>
`ssize_t (*write) (struct file *filp, const char __user *buf, size_t count, loff_t *pos);`
- Returns number of bytes written
- Must check for bad or invalid data, the book outlines what to do here on pg. 157. 
