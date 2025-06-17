# Makefile for a simple Linux kernel module that implements file operations

EXTRA_CFLAGS = -Wall
obj-m = file_ops.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean