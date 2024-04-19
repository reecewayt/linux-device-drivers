#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <unistd.h>     
#include <errno.h>      
#include <string.h>


int main(void){
    int sys_val;    //stores system reads and writes from device driver
    int fd;         //file descriptor for driver
    char *dev_path = "/dev/first-char-driver"; //path to device file
    int num_bytes; 

    //open device with read and write access 
    fd = open(dev_path, O_RDWR);
    if(fd < 0){
        perror("Failed to open the device...\n");
        printf("Error description: %s\n", strerror(errno));
        return errno; 
    }

    printf("Device is open, lets read the current value held in the device...\n");

    num_bytes = read(fd, &sys_val, sizeof(int));
    if(num_bytes < sizeof(int)){
        perror("Failed to read correct number of bytes from device...\n");
        printf("Error description: %s\n", strerror(errno));
        return errno;
    }
    printf("Current system value read from device: %d\n", sys_val);

    printf("\nLet's write a new value, enter an integer value...\n");
    scanf("%d", &sys_val);
    printf("You entered %d...\n", sys_val);

    num_bytes = write(fd, &sys_val, sizeof(int));
    if(num_bytes < sizeof(int)){
      perror("Failed to write correct number of bytes from device...\n");
        printf("Error description: %s\n", strerror(errno));
        return errno;  
    }

    printf("Write successful, lets check that everything worked by reading again...\n");
    num_bytes = read(fd, &sys_val, sizeof(int));
    if(num_bytes < sizeof(int)){
        perror("Failed to read correct number of bytes from device...\n");
        printf("Error description: %s\n", strerror(errno));
        return errno;
    }
    printf("Current system value read from device: %d\n", sys_val);

    printf("Program done, closing device now...\n");
    close(fd);



    return 0; 
}