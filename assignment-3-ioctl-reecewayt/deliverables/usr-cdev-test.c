#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <unistd.h>     
#include <errno.h>      
#include <string.h>
#include <sys/ioctl.h>
#include <signal.h>

#define IOC_MAGIC_NUM 'z'
#define MY_IOC_R_CMD _IOR(IOC_MAGIC_NUM, 1, int)
#define MY_IOC_W_CMD _IOW(IOC_MAGIC_NUM, 2, int)

int fd;         //file descriptor for driver

// Signal handler for SIGINT (Ctrl+C)
void sigint_handler(int sig_num) {
    printf("\nReceived Ctrl-C (SIGINT), closing device and exiting...\n");
    if (fd >= 0) {
        close(fd);  // Close the device file if it's open
    }
    exit(0);  // Exit the program
}

int main(int argc, char *argv[]){
    if(argc != 2) {
        fprintf(stderr, "Usage: User did not provide two arguments\n");
        return 1; 
    }
    int sys_val;    //stores system reads and writes from device driver
    int temp;      // used to check system values
    char *dev_path = "/dev/first-char-driver"; //path to device file
    int num_bytes; 

    //initialize signal handler callback
    signal(SIGINT, sigint_handler);

    //open device with read and write access 
    fd = open(dev_path, O_RDWR);
    if(fd < 0){
        perror("Failed to open the device...\n");
        printf("Error description: %s\n", strerror(errno));
        return errno; 
    }

    sys_val = atoi(argv[1]); //convert cmdl argument to integer

    if(ioctl(fd, MY_IOC_W_CMD, &sys_val)){
        perror("Failed to write to device using ioctl \n");
        printf("Error description: %s\n", strerror(errno));
    }
    printf("Device is open, lets test the ioctl commands...\n");

    if(ioctl(fd, MY_IOC_R_CMD, &temp)){
        perror("Failed to write to device using ioctl \n");
        printf("Error description: %s\n", strerror(errno));
    }

    printf("You initialized the device with %d\n", sys_val);
    printf("Let's try some more values, press Ctrl+C to exit program\n\n");

    while(1){
        printf("What integer would you like to write using ioctl: ");
        scanf("%d", &sys_val);

        if(ioctl(fd, MY_IOC_W_CMD, &sys_val)){
            perror("Failed to write to device using ioctl \n");
            printf("Error description: %s\n", strerror(errno));
        }
        printf("You wrote %d to device using ioctl...\n", sys_val);
        if(ioctl(fd, MY_IOC_R_CMD, &temp)){
            perror("Failed to write to device using ioctl \n");
            printf("Error description: %s\n", strerror(errno));
        }
        printf("The value read from device using ioctl is %d...go again...\n", temp);
    }

    return 0; 
}