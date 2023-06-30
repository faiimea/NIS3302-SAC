#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <asm/pgtable.h>
#include <asm/ptrace.h>
#include <asm/uaccess.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/fs_struct.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/unistd.h>

#define FILE_PATH "/home/faii/Desktop/a.txt"
#define CONTENT "reboot here\n"

/*
** module macros
*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("infosec-sjtu");
MODULE_DESCRIPTION("hook sys_call_table");

typedef void (*demo_sys_call_ptr_t)(void);
// asmlinkage
// pt_regs store the regs_info in stack
// function pointer and the para is regs(pt_regs)
typedef asmlinkage long (*orig_openat_t)(struct pt_regs *regs);
typedef asmlinkage long (*orig_read_t)(struct pt_regs *regs);
typedef asmlinkage long (*orig_reboot_t)(struct pt_regs *regs);

demo_sys_call_ptr_t *get_syscall_table(void);

int AuditOpenat(struct pt_regs *, char *pathname, int ret);

void netlink_release(void);
void netlink_init(void);

demo_sys_call_ptr_t *sys_call_table = NULL;
orig_openat_t orig_openat = NULL;
orig_reboot_t orig_reboot = NULL;

unsigned int level;
pte_t *pte;

asmlinkage long hacked_openat(struct pt_regs *regs) {

  long ret;
  char buffer_open[PATH_MAX];
  long nbytes;
  unsigned long flag = regs->di;

  // }-> pathname --> fullname
  nbytes = strncpy_from_user(buffer_open, (char *)regs->si, PATH_MAX);
  // printk("Info:   hooked sys_openat(), file right:%lu",flag);
  //  if (strncmp(buffer, "/home/faii/Desktop/", strlen("/home/faii/Desktop/"))
  //  == 0) { printk("Info:   hooked sys_openat(), file name:%s(%ld
  //  bytes)",buffer,nbytes);
  //  }
  ret = orig_openat(regs);
  // 用于记录openat系统调用信息的函数
  AuditOpenat(regs, buffer_open, ret);

  return ret;
}

int add_line_to_file(const char *filename, const char *line)
{
    struct file *file;
    loff_t pos;
    ssize_t ret;

    file = filp_open(filename, O_RDWR | O_APPEND, 0);
    if (IS_ERR(file))
        return PTR_ERR(file);

    pos = file->f_inode->i_size;

    ret = kernel_write(file, line, strlen(line), &pos);
    if (ret >= 0)
        ret = kernel_write(file, "\n", 1, &pos);  // Add a newline character

    filp_close(file, NULL);

    return ret;
}

asmlinkage long hacked_reboot(struct pt_regs *regs) {
  long ret=0;
  printk("FAIINFO:HERE REBOOT!!!");
  add_line_to_file("/home/faii/Desktop/a.txt", "now reboot");
  //ret=orig_reboot(regs);
  return ret;

}

static int __init audit_init(void) {

  sys_call_table = get_syscall_table();

  printk("Info: sys_call_table found at %lx\n", (unsigned long)sys_call_table);
  // HERE!!!!
  // Hook Sys Call Openat
  // 将sys_call_table中的openat系统调用的原始函数保存到orig_openat变量中。
  // function address
  orig_openat = (orig_openat_t)sys_call_table[__NR_openat];
  orig_reboot = (orig_reboot_t)sys_call_table[__NR_reboot];
  printk("Info:  orginal openat:%lx\n", (long)orig_openat);
  printk("Info:  orginal reboot:%lx\n", (long)orig_reboot);

  pte = lookup_address((unsigned long)sys_call_table, &level);
  // 查找sys_call_table对应的页表项，并将其设置为可写。
  set_pte_atomic(pte, pte_mkwrite(*pte));
  printk("Info: Disable write-protection of page with sys_call_table\n");
  // 将sys_call_table中的openat系统调用的函数指针设置为指向钩子函数hacked_openat
  // demo_sys_call_ptr_t is void* address
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)hacked_openat;
  sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)hacked_reboot;
  // 将页表项恢复为只读。
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  printk("Info: sys_call_table hooked!\n");
  // 初始化一个用于通信的Netlink套接字。
  // netlink is not my job :)
  netlink_init();
  return 0;
}

static void __exit audit_exit(void) {
  // 查找sys_call_table对应的页表项，并将其设置为可写。
  pte = lookup_address((unsigned long)sys_call_table, &level);
  set_pte_atomic(pte, pte_mkwrite(*pte));
  // 将sys_call_table中的openat系统调用的函数指针恢复为指向原始函数。
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)orig_openat;
  sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)orig_reboot;
  // 将页表项恢复为只读。
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  // 释放Netlink套接字。
  netlink_release();
}

// 模块的初始化函数+清理函数。
module_init(audit_init);
module_exit(audit_exit);
