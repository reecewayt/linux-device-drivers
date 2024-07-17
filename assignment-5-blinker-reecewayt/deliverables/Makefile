obj-m := my-e1000-drv-timer.o
KERNEL_DIR := /lib/modules/$(shell uname -r)/build
EXTRA_CFLAGS += -msoft-float -msse -msse2 -msse4.1 -msse4.2
BUILDS := \( -name '*.cmd' -o -name '*.mod*' -o -name '*.o' -o -name '*.ko' -o -name '*.symvers' -o -name '*.order' \)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
	find . $(BUILDS) -exec mv {} build/ \;

test:	
	gcc -o usr_space_led_test.o usr_space_led_test.c
	find . $(BUILDS) -exec mv {} build/ \;

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean