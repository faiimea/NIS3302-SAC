#!/bin/bash
sudo insmod AuditModule.ko audit_connect=1 audit_bind=0 audit_sendto=1 audit_recvfrom=1 audit_socket=1 audit_finit_module=1 audit_delete_module=1 audit_mount=1 audit_umount2=1 audit_execve=1 audit_openat=1 audit_unlinkat=1 audit_write=1 audit_close=1 audit_read=1 audit_mknodat=1 audit_path=/home/faii/Desktop/TEST 
