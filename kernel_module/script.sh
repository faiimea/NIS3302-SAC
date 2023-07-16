#!/bin/bash
sudo insmod AuditModule.ko connect=0 bind=0 sendto=0 recvfrom=0 socket=0 finit_module=1 delete_module=1 mount=0 umount2=1 execve=1 openat=1 unlinkat=1 write=1 close=1 read=1 mknodat=1 
