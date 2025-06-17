# Hello world kernel module

EXTRA_CFLAGS = -Wall
obj-m = dev_file.o

KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean