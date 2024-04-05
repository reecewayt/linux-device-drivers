**Reece Wayt <br>
Assignment #1 <br>
ECE 373, Spring 2024** <br>
------

# A. Information Gathering

1. Look at the man page for the "open()" system call, found in section two of the manpages. <br>
- List the BUGS when using the system call.<br>
    + Its not possible to enable signal-driven by specifying I/O when calling open(); 
    + You must check two error codes, EISDIR, and ENOENT, when determining if kernel support O-TMPFILE.
    + When both O_CREATE and O_DIRECTORY are specified as flags but the file specified by pathname does not exit, open() will create a regular file instead of directory.
- What files need to be included to use this function? <br>
```
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
```
- List the first three related system calls to open(). <br>
    + chmod, chown, close
- Choose one of the system calls from above and list its bugs (also list what system call you
chose) and files needing to be included to use the system call.
    + Bugs: 
        * No bugs are listed in the man page
    + Include directives: 
    ```
    #include <sys/stat.h>
    ```
2. Use https://elixir.bootlin.com/ to search for the following:
- Search for "usb_device". In what file is the structure defined, and what are the first five
members of the struct?
    + File: `include/linux/usb.h`
    + Members: `devnum`, `devpath[16]`, `route`, `state`, `speed`
- In what header file is the type declared for the 5th member of the struct? (hint: don't look in
test tools or staging directories)
    + `enum usb_device_speed` is define here -> `include/uapi/linux/usb/ch9.h`
- Include the entire enumeration declaration from above
```C
enum usb_device_speed {
	USB_SPEED_UNKNOWN = 0,			    /* enumerating */
	USB_SPEED_LOW, USB_SPEED_FULL,		/* usb 1.1 */
	USB_SPEED_HIGH,				        /* usb 2.0 */
	USB_SPEED_WIRELESS,			        /* wireless (usb 2.5) */
	USB_SPEED_SUPER,			        /* usb 3.0 */
	USB_SPEED_SUPER_PLUS,			    /* usb 3.1 */
};
```

# B. Basic Linux Use:
1. Log onto an Ubuntu Linux or other system (presumably your VirtualBox virtual machine)
2. Under each heading in the toolbar at the top, pop up and note the menu and sub menu tools/topics.
Note where you find Terminal and the gedit Text Editor, or any other text editor of your choice.
3. Click on the Terminal entry to bring up a command shell line. This should be familiar to you from
working with Cygwin in ECE 371/372. You will use Terminal to enter Linux commands.
4. To get an overview of Linux structure and commands, go to
http://linuxcommand.org/lc3_learning_the_shell.php . Study the “Learning the shell”
section carefully and make some notes on basic commands to make a directory, determine current
directory, change to a different directory, do a long or short form listing of the files in a directory,
copy a file, etc.
5. The “script” command is a useful way to make a record of all the commands you enter at the
command shell and the results of those commands. Script records the commands, etc. in a file
called typescript in your current directory. You terminate “script” with CTRL-D or 'exit'. You can
then edit/print the typescript file. You will be turning in a copy of the typescript file for a shell
practice session.
6. For a start with the basic shell commands, enter script at the command prompt and then:
A. Enter the command to determine the current directory
B. Enter the command to command to show the files in that directory with file permissions.
C. Enter a command to make a directory called ECE373.
D. Change the working directory to that directory.
E. Exit script with CTRL D.
7. Find the gedit program under Application|Accessories and open a gedit window. Find the File|Open
menu command and find the typescript file you created. Click on Open and you should see the
typescript file in the window. It likely contains some control characters along with the commands
but you can easily edit these out, if you 



