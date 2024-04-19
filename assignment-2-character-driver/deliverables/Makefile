obj-m = first-char-driver.o
KERNEL_DIR = /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) modules

test:	
	gcc -o usr-cdev-test usr-cdev-test.c

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(shell pwd) clean
