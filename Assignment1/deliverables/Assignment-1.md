**Reece Wayt <br>
Assignment #1 Deliverables <br>
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
- See file `basic-linux-use-script.txt` for script output.

# C. Basic C Programming in Linux: 
- Example program is `fah-to-cel.c` below is the source code: 
```C
#include <stdio.h>

int main() {
    double fahrenheit, celsius;

    printf("Enter temperature in Fahrenheit: ");
    scanf("%lf", &fahrenheit);

    // Convert Fahrenheit to Celsius
    celsius = (fahrenheit - 32) * 5 / 9;

    // Display the result
    printf("%.2f Fahrenheit is %.2f Celsius.\n", fahrenheit, celsius);

    return 0;
}
```

- Script output is found in the file `basic-c-program-script.txt`, and it is also highlighted below: 
```bash
reecewayt@fossa-zen:~/ece373/linux-device-drivers/Assignment-1$ cat basic-c-program-script.txt 
Script started on 2024-04-04 17:57:20-07:00 [TERM="xterm-256color" TTY="/dev/pts/3" COLUMNS="250" LINES="24"]
reecewayt@fossa-zen:~/ece373/linux-device-drivers/Assignment-1$ gcc -g -o fah-to-cel fah-to-cel.c
reecewayt@fossa-zen:~/ece373/linux-device-drivers/Assignment-1$ ./fah-to-cel
Enter temperature in Fahrenheit: 96.3
96.30 Fahrenheit is 35.72 Celsius.
reecewayt@fossa-zen:~/ece373/linux-device-drivers/Assignment-1$ exit
exit

Script done on 2024-04-04 17:57:58-07:00 [COMMAND_EXIT_CODE="0"]
```
- Notes on GNU Debugger `man gdb` <br>
    + To invoke gdb you will usually call it with an executable program, i.e. `gdb program.out`.<br>
    + You can also run gdb for a running program using a process ID as a second argument, i.e. `gdb -p 1234`.  

- Main gdb commands 

    + `break`: sets a breakpoint at a function or line number -> `break myfunc`.
    + `run`: starts the program, you can pass command line arguments after run. 
    + `next`: Execute next line of code, stepping over function calls. It does not enter functions. 
    + `step`: executes next line of code, but will enter functions. 
    + `print`: displays the value of an expression -> `print myNum` will print the value held by myNum for inspection
    + `quit`: exits from the debugger


# D. Hello, Kernel: Loading a module into the Kernel





