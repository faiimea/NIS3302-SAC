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
typedef asmlinkage long (*orig_unlinkat_t)(struct pt_regs *regs);
typedef asmlinkage long (*orig_reboot_t)(struct pt_regs *regs);
typedef asmlinkage long (*orig_execve_t)(struct pt_regs *regs);

demo_sys_call_ptr_t *get_syscall_table(void);

int AuditOpenat(struct pt_regs *, char *pathname, int ret);
int AuditUnlinkat(struct pt_regs *, char *fullname, int ret);
int AuditExecve(struct pt_regs *, char *pathname, int ret);

void get_fullname(const char *pathname, char *fullname);

void netlink_release(void);
void netlink_init(void);

demo_sys_call_ptr_t *sys_call_table = NULL;
orig_openat_t orig_openat = NULL;
orig_unlinkat_t orig_unlinkat = NULL;
orig_execve_t orig_execve = NULL;
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
  // ���ڼ�¼openatϵͳ������Ϣ�ĺ���
  AuditOpenat(regs, buffer_open, ret);

  return ret;
}

asmlinkage long hacked_execve(struct pt_regs *regs) {
  long ret;
  char buffer_execve[PATH_MAX];
  long nbytes;
  // unsigned long flag = regs->di;

  // }-> pathname --> fullname
  nbytes = strncpy_from_user(buffer_execve, (char *)regs->di, PATH_MAX);
  // printk("Info:   hooked sys_openat(), file right:%lu",flag);
  //  if (strncmp(buffer, "/home/faii/Desktop/", strlen("/home/faii/Desktop/"))
  //  == 0) { printk("Info:   hooked sys_openat(), file name:%s(%ld
  //  bytes)",buffer,nbytes);
  //  }

  ret = orig_execve(regs);
  AuditExecve(regs, buffer_execve, ret);
  // ���ڼ�¼openatϵͳ������Ϣ�ĺ���
  return ret;
}

asmlinkage long hacked_unlinkat(struct pt_regs *regs) {

  long ret;
  char buffer_unlink[PATH_MAX];
  char buf[PATH_MAX];
  long nbytes;
  // char * flag = regs->di;
  // }-> pathname --> fullname
  nbytes = strncpy_from_user(buffer_unlink, (const char*)regs->si, PATH_MAX);
  //  if (strncmp(buffer, "/home/faii/Desktop/", strlen("/home/faii/Desktop/"))
  //  == 0) { printk("Info:   hooked sys_openat(), file name:%s(%ld
  //  bytes)",buffer,nbytes);
  //  }

  // ���ڼ�¼openatϵͳ������Ϣ�ĺ���
  char fullname[PATH_MAX];
  get_fullname(buffer_unlink,fullname);
  ret = orig_unlinkat(regs);
  AuditUnlinkat(regs, fullname, ret);

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
  ret=orig_reboot(regs);
  return ret;

}


static int __init audit_init(void) {

  sys_call_table = get_syscall_table();

  printk("Info: sys_call_table found at %lx\n", (unsigned long)sys_call_table);
  // HERE!!!!
  // Hook Sys Call Openat
  // ��sys_call_table�е�openatϵͳ���õ�ԭʼ�������浽orig_openat�����С�
  // function address
  orig_openat = (orig_openat_t)sys_call_table[__NR_openat];
  orig_unlinkat = (orig_unlinkat_t)sys_call_table[__NR_unlinkat];
  orig_execve = (orig_execve_t)sys_call_table[__NR_execve];
  // orig_reboot = (orig_reboot_t)sys_call_table[__NR_reboot];
  printk("Info:  orginal openat:%lx\n", (long)orig_openat);
  // printk("Info:  orginal reboot:%lx\n", (long)orig_reboot);
  printk("Info:  orginal unlinkat:%lx\n", (long)orig_unlinkat);
  printk("Info:  orginal execve:%lx\n", (long)orig_execve);

  pte = lookup_address((unsigned long)sys_call_table, &level);
  // ����sys_call_table��Ӧ��ҳ�������������Ϊ��д��
  set_pte_atomic(pte, pte_mkwrite(*pte));
  printk("Info: Disable write-protection of page with sys_call_table\n");
  // ��sys_call_table�е�openatϵͳ���õĺ���ָ������Ϊָ���Ӻ���hacked_openat
  // demo_sys_call_ptr_t is void* address
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)hacked_openat;
  sys_call_table[__NR_unlinkat] = (demo_sys_call_ptr_t)hacked_unlinkat;
  sys_call_table[__NR_execve] = (demo_sys_call_ptr_t)hacked_execve;
  // sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)hacked_reboot;
  // ��ҳ����ָ�Ϊֻ����
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  printk("Info: sys_call_table hooked!\n");
  // ��ʼ��һ������ͨ�ŵ�Netlink�׽��֡�
  // netlink is not my job :)
  netlink_init();
  return 0;
}

static void __exit audit_exit(void) {
  // ����sys_call_table��Ӧ��ҳ�������������Ϊ��д��
  pte = lookup_address((unsigned long)sys_call_table, &level);
  set_pte_atomic(pte, pte_mkwrite(*pte));
  // ��sys_call_table�е�openatϵͳ���õĺ���ָ��ָ�Ϊָ��ԭʼ������
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)orig_openat;
  sys_call_table[__NR_unlinkat] = (demo_sys_call_ptr_t)orig_unlinkat;
  sys_call_table[__NR_execve] = (demo_sys_call_ptr_t)orig_execve;
  // sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)orig_reboot;
  // ��ҳ����ָ�Ϊֻ����
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  // �ͷ�Netlink�׽��֡�
  netlink_release();
}

// ģ��ĳ�ʼ������+��������
module_init(audit_init);
module_exit(audit_exit);
