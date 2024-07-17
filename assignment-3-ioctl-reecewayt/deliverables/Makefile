obj-m = first-char-driver.o
KERNEL_DIR = /lib/modules/$(shell uname -r)/build
BUILDS = \( -name '*.cmd' -o -name '*.mod*' -o -name '*.o' -o -name '*.ko' -o -name '*.symvers' -o -name '*.order' \)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules
	find . $(BUILDS) -exec mv {} build/ \;

test:	
	gcc -o usr-cdev-test.o usr-cdev-test.c

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean

