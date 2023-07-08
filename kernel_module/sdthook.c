#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <linux/net.h>
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

MODULE_LICENSE("GPL");
MODULE_AUTHOR("infosec-sjtu");
MODULE_DESCRIPTION("hook sys_call_table");

typedef void (*demo_sys_call_ptr_t)(void);

typedef asmlinkage long (*sys_call_t)(struct pt_regs *regs);


demo_sys_call_ptr_t *get_syscall_table(void);

int AuditOpenat(struct pt_regs *, char *pathname, int ret);
int AuditConnect(struct pt_regs * regs,char * netbuf,int ret);
int AuditUnlinkat(struct pt_regs *, char *fullname, int ret);
int AuditExecve(struct pt_regs *, char *pathname, int ret);
int AuditReboot(struct pt_regs * regs,int ret);
int AuditSocket(struct pt_regs * regs,int ret,int a,int b,int c);
int Auditfinitmodule(char * pathname);
int Auditdeletemodule(char * modulename);

void get_fullname(const char *pathname, char *fullname);
void netlink_release(void);
void netlink_init(void);

demo_sys_call_ptr_t *sys_call_table = NULL;
sys_call_t orig_connect = NULL;
sys_call_t orig_openat = NULL;
sys_call_t orig_reboot = NULL;
sys_call_t orig_execve = NULL;
sys_call_t orig_unlinkat = NULL;
sys_call_t orig_finitmodule = NULL;
sys_call_t orig_deletemodule = NULL;
sys_call_t orig_openat_t = NULL;
sys_call_t orig_socket = NULL;


unsigned int level;
pte_t *pte;

int get_info_from_fd(unsigned int fd, unsigned long * ino, uid_t * uid, int *type,char* buf)
{
    struct file *file_p = NULL;
    struct inode * f_inode = NULL;
    struct fd f;
    char* buff;


    // 获取 uid	
    *uid = current_uid().val;   // unsigned int
    // 获取fd对应的file struct
    // file_p = fget_raw(fd);
    f = fdget(fd);
    file_p = f.file;
    if (!file_p)
    {
        // 获取失败
        return -1;
    }

    // 获取 ino
    f_inode = file_p->f_inode;
    *ino = f_inode->i_ino;

    //get path

    buff=d_path(&file_p->f_path, buf, PATH_MAX);
    strcpy(buf,buff);

    return 0;
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

void print_ip_address(const unsigned int *ip)
{
    unsigned char bytes[4];
    bytes[0] = *ip & 0xFF;
    bytes[1] = (*ip >> 8) & 0xFF;
    bytes[2] = (*ip >> 16) & 0xFF;
    bytes[3] = (*ip >> 24) & 0xFF;

    printk(KERN_INFO "IP Address: %d.%d.%d.%d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
}

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

asmlinkage long hacked_reboot(struct pt_regs *regs) {
  long ret=0;
  printk("FAIINFO:HERE REBOOT!!!");
  add_line_to_file("/home/faii/Desktop/a.txt", "now reboot");
  ret=orig_reboot(regs);
  return ret;

}

asmlinkage long hacked_socket(struct pt_regs *regs) {
    long ret = 0;
    int family = (int)regs->di;
    int type = (int)regs->si;
    int protocol = (int)regs->dx;
    ret = orig_socket(regs);
    AuditSocket(regs,ret,family,type,protocol);
    return ret;
}

asmlinkage long hacked_connect(struct pt_regs *regs) {
    long ret = 0;
    int sockfd = (int)regs->di;
    //unsigned long addr_val = regs->si;
    struct sockaddr *addr = (struct sockaddr *)regs->si;
    int addrlen = (int)regs->dx;
    //printk("hi\n");
    char buffer[14];
    //print_sockaddr(addr);
    unsigned short family = addr->sa_family;

    strncpy_from_user(buffer, addr->sa_data, 14);
    const unsigned short *port = (const unsigned short *)(buffer + 2);
    unsigned short port_tmp=*port;
    const unsigned int *ip = (const unsigned int *)(buffer + 4);
    int ip_tmp=*ip;
    //if(port_tmp){
    //if(buffer)  printk("Info:%s\n",buffer);
    //printk("Port: %hu\n", port_tmp);
    //print_ip_address(ip);}
    ret = orig_connect(regs);
    AuditConnect(regs,buffer,ret);
    return ret;
}

asmlinkage long hacked_execve(struct pt_regs *regs) {
  long ret;
  char buffer_execve[PATH_MAX];
  long nbytes;
  nbytes = strncpy_from_user(buffer_execve, (char *)regs->di, PATH_MAX);
  ret = orig_execve(regs);
  AuditExecve(regs, buffer_execve, ret);
  return ret;
}

asmlinkage long hacked_unlinkat(struct pt_regs *regs) {

  long ret;
  char buffer_unlink[PATH_MAX];
  char buf[PATH_MAX];
  long nbytes;

  nbytes = strncpy_from_user(buffer_unlink, (const char*)regs->si, PATH_MAX);

  char fullname[PATH_MAX];
  get_fullname(buffer_unlink,fullname);
  ret = orig_unlinkat(regs);
  AuditUnlinkat(regs, fullname, ret);

  return ret;
}

asmlinkage long hacked_finitmodule(struct pt_regs *regs)
{
    long ret;
    char buffer[PATH_MAX];
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;

    get_info_from_fd(regs->di, &ino, &uid, &f_type,buffer);  

    if(strlen(buffer)>0){
        printk("ModuleInstalled:%s\n",buffer);
        Auditfinitmodule(buffer);
    }    
    ret = orig_finitmodule(regs);

    return ret;
}

asmlinkage long hacked_deletemodule(struct pt_regs *regs){
    long ret;
    char buffer[PATH_MAX];


    strncpy_from_user(buffer,(char*)regs->di,PATH_MAX);

    if(strlen(buffer)>0){
        printk("ModuleDeleted:%s\n",buffer);
        Auditdeletemodule(buffer);
    }
    else{printk(KERN_ERR "ModuleDeletedFailedtoResolve\n");}
    ret = orig_deletemodule(regs);
    return ret;
}

static int __init audit_init(void) {

  sys_call_table = get_syscall_table();
  orig_openat = (sys_call_t)sys_call_table[__NR_openat];
  orig_reboot = (sys_call_t)sys_call_table[__NR_reboot];
  orig_connect = (sys_call_t)sys_call_table[__NR_connect];
  orig_unlinkat = (sys_call_t)sys_call_table[__NR_unlinkat];
  orig_execve = (sys_call_t)sys_call_table[__NR_execve];
  orig_finitmodule = (sys_call_t) sys_call_table[__NR_finit_module];
  orig_deletemodule = (sys_call_t) sys_call_table[__NR_delete_module];
  orig_socket = (sys_call_t) sys_call_table[__NR_socket];
  pte = lookup_address((unsigned long)sys_call_table, &level);
  set_pte_atomic(pte, pte_mkwrite(*pte));
  sys_call_table[__NR_connect] = (demo_sys_call_ptr_t)hacked_connect;
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)hacked_openat;
  sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)hacked_reboot;
  sys_call_table[__NR_unlinkat] = (demo_sys_call_ptr_t)hacked_unlinkat;
  sys_call_table[__NR_execve] = (demo_sys_call_ptr_t)hacked_execve;
  sys_call_table[__NR_finit_module] = (demo_sys_call_ptr_t)hacked_finitmodule;
  sys_call_table[__NR_delete_module] = (demo_sys_call_ptr_t)hacked_deletemodule;
  sys_call_table[__NR_socket] = (demo_sys_call_ptr_t)hacked_socket;
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  netlink_init();
  return 0;
}

static void __exit audit_exit(void) {
  pte = lookup_address((unsigned long)sys_call_table, &level);
  set_pte_atomic(pte, pte_mkwrite(*pte));
  sys_call_table[__NR_connect] = (demo_sys_call_ptr_t)orig_connect;
  sys_call_table[__NR_openat] = (demo_sys_call_ptr_t)orig_openat;
  sys_call_table[__NR_reboot] = (demo_sys_call_ptr_t)orig_reboot;
  sys_call_table[__NR_unlinkat] = (demo_sys_call_ptr_t)orig_unlinkat;
  sys_call_table[__NR_execve] = (demo_sys_call_ptr_t)orig_execve;
  sys_call_table[__NR_finit_module] = (demo_sys_call_ptr_t)orig_finitmodule;
  sys_call_table[__NR_delete_module] = (demo_sys_call_ptr_t)orig_deletemodule;
  sys_call_table[__NR_socket] = (demo_sys_call_ptr_t)orig_socket;
  set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
  netlink_release();
}

module_init(audit_init);
module_exit(audit_exit);