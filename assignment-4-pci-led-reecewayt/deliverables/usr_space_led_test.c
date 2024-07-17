#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      
#include <unistd.h>  
#include <stdint.h>   
#include <errno.h>      
#include <string.h>
#include <signal.h>

// Giga Ethernet Control Registers
/* LED Control Reg Break Down, pg. 304 of Intel Dev Manual
    -------------------------------------
    |31 - 24 | 23 - 16 | 15 - 8 | 7 - 0 |
    | LED 3  | LED 2   | LED 1  | LED 0 |
    ------------------------------------- 
*/
#define E1000_LEDCTL                0x00E00 
#define E1000_LEDCTL_SHIFT  		8
//mode bits are [3:0]
#define E1000_LEDCTL_MODE_MASK		0x0F

#define E1000_LEDCTL_MODE_LED_ON	0xE //1110b led on
#define E1000_LEDCTL_MODE_LED_OFF	0xF //1111b led off

#define LEFT_SHIFT(x, pos) ((x << num))

int fd; //file descriptor of /dev entry

// Signal handler for SIGINT (Ctrl+C)
void sigint_handler(int sig_num) {
    printf("\nReceived Ctrl-C (SIGINT), closing device and exiting...\n");
    if (fd >= 0) {
        close(fd);  // Close the device file if it's open
    }
    exit(0);  // Exit the program
}

/**
 * @brief Main function to control Ethernet LED registers.
 *        It opens the device, reads the current register value, and iterates through LEDs to turn them on.
 *        Includes error handling and signal handling for a clean exit.
 */
int main(){
    uint32_t curr_reg_val; //holds current reg value of led register
    uint32_t new_reg_val; //holds new reg value of led register
    int ret_val;            
    //initialize signal handler callback
    signal(SIGINT, sigint_handler);

    fd = open("/dev/my-e1000", O_RDWR);
    if(fd < 0){
        perror("Failed to open the device...");
        printf("Error description: %s\n", strerror(errno));
        return errno; 
    }

    //read current register value
    ret_val = read(fd, &curr_reg_val, sizeof(uint32_t));
    if(ret_val < sizeof(uint32_t)){
        perror("Failed to read the device...");
        printf("Error description: %s\n", strerror(errno));
        close(fd);  // Close the device file
        return errno;
    }

    printf("Current register value: 0x%08X\n", curr_reg_val);

    for(int i = 0; i < 4; i++){
        new_reg_val = curr_reg_val & ~(E1000_LEDCTL_MODE_MASK << (i * E1000_LEDCTL_SHIFT));
        new_reg_val |= E1000_LEDCTL_MODE_LED_ON << (i * E1000_LEDCTL_SHIFT); //set the mode to LED_ON
        printf("Setting LED %d to ON\n", i);
        ret_val = write(fd, &new_reg_val, sizeof(uint32_t));
        if(ret_val < sizeof(uint32_t)){
            perror("Failed to write the device...");
            printf("Error description: %s\n", strerror(errno));
            close(fd);  // Close the device file
            return errno;
        }
        ret_val = read(fd, &curr_reg_val, sizeof(uint32_t));
        if(ret_val < sizeof(uint32_t)){
            perror("Failed to read the device...");
            printf("Error description: %s\n", strerror(errno));
            close(fd);  // Close the device file
            return errno;
        }
        printf("Register value read: 0x%08X\n", curr_reg_val);
        sleep(2); //sleep for one second
    }

    printf("All LEDs have been turned on, goodbye...\n");

    close(fd);  // Close the device file before exiting
    return 0; 
}
