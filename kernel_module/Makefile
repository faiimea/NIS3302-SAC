CONFIG_MODULE_SIG=n
obj-m:=AuditModule.o
AuditModule-objs   :=sdthook.o syscalltable.o netlinkp.o 
KDIR   := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(RM) -rf .*.cmd *.mod.c *.o *.ko .tmp*
update:
	sudo rmmod AuditModule.ko
	sudo dmesg -c
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo insmod AuditModule.ko
	sudo dmesg
build:
	sudo dmesg -c
	$(MAKE) -C $(KDIR) M=$(PWD) modules
	sudo insmod AuditModule.ko
	sudo dmesg

