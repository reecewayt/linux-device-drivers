#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


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
int main() {         
    uint32_t curr_blink_rate;
    uint32_t new_blink_rate;
    int ret_val;
    //initialize signal handler callback
    signal(SIGINT, sigint_handler);

    fd = open("/dev/my-e1000-drv-timer", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device...");
        printf("Error description: %s\n", strerror(errno));
        return errno; 
    }

    ret_val = read(fd, &curr_blink_rate, sizeof(uint32_t));
    if (ret_val < 0) {
        perror("Failed to read the blink rate from the device...");
        printf("Error Description: %s\n", strerror(errno));
    } else {
        printf("Current blink rate: %u\n", curr_blink_rate);
    }

    while (1) {
        printf("Enter a new blink rate...\n");
        scanf("%u", &new_blink_rate);
        ret_val = write(fd, &new_blink_rate, sizeof(uint32_t));

        if (ret_val < 0) {
            perror("Failed to write the new blink rate to the device...");
            printf("Error Description: %s\n", strerror(errno));
        }

        sleep(5);
        ret_val = read(fd, &curr_blink_rate, sizeof(uint32_t));

        if (ret_val < 0) {
            perror("Failed to read the blink rate from the device...");
            printf("Error Description: %s\n", strerror(errno));
        } else {
            printf("Last value entered was %u... Try again\n", curr_blink_rate);
        }
        
    }

    close(fd);  // Close the device file before exiting
    return 0; 
}
