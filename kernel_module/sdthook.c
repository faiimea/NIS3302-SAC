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
#include <linux/moduleparam.h>

#define FILE_PATH "/home/test/Desktop/a.txt"
#define CONTENT "reboot here\n"
#define MAX_COUNT 1024

MODULE_LICENSE("GPL");
MODULE_AUTHOR("infosec-sjtu");
MODULE_DESCRIPTION("hook sys_call_table");

typedef void (*demo_sys_call_ptr_t)(void);

typedef asmlinkage long (*sys_call_t)(struct pt_regs *regs);

static char *path = "/home/test/Desktop";
module_param(path, charp, S_IRUSR);

static int audit_connect = 1;
module_param(audit_connect, int, S_IRUSR);
static int audit_bind = 1;
module_param(audit_bind, int, S_IRUSR);
static int audit_sendto = 1;
module_param(audit_sendto, int, S_IRUSR);
static int audit_recvfrom = 1;
module_param(audit_recvfrom, int, S_IRUSR);
static int audit_socket = 1;
module_param(audit_socket, int, S_IRUSR);
static int audit_finit_module = 1;
module_param(audit_finit_module, int, S_IRUSR);
static int audit_delete_module = 1;
module_param(audit_delete_module, int, S_IRUSR);
static int audit_mount = 1;
module_param(audit_mount, int, S_IRUSR);
static int audit_umount2 = 1;
module_param(audit_umount2, int, S_IRUSR);
static int audit_execve = 1;
module_param(audit_execve, int, S_IRUSR);
static int audit_openat = 1;
module_param(audit_openat, int, S_IRUSR);
static int audit_unlinkat = 1;
module_param(audit_unlinkat, int, S_IRUSR);
static int audit_write = 1;
module_param(audit_write, int, S_IRUSR);
static int audit_close = 1;
module_param(audit_close, int, S_IRUSR);
static int audit_read = 1;
module_param(audit_read, int, S_IRUSR);
static int audit_mknodat = 1;
module_param(audit_mknodat, int, S_IRUSR);

demo_sys_call_ptr_t *get_syscall_table(void);

int AuditOpenat(struct pt_regs *, char *pathname, int ret, char *path);
int AuditConnect(struct pt_regs *regs, char *netbuf, int ret);
int AuditBind(struct pt_regs *regs, char *netbuf, int ret);
int AuditSendto(struct pt_regs *regs, char *netbuf, int ret);
int AuditRecvfrom(struct pt_regs *regs, char *netbuf, int ret);
int AuditUnlinkat(struct pt_regs *, char *fullname, int ret, char *path);
int AuditExecve(struct pt_regs *, char *pathname, int ret, char *path);
int AuditRead(struct pt_regs *, char *pathname, int ret, char *path);
int AuditWrite(struct pt_regs *, char *pathname, int ret, char *path);
int AuditClose(struct pt_regs *, char *pathname, int ret, char *path);
int AuditMknodat(struct pt_regs *, char *pathname, int ret, char *path);
int AuditReboot(struct pt_regs *regs, int ret);
int AuditSocket(struct pt_regs *regs, int ret, int a, int b, int c);
int Auditfinitmodule(char *pathname, int ret);
int Auditdeletemodule(char *modulename, int ret);
int Auditmount(char *source, char *filesystemtype, char *target, int ret);
int Auditumount(char *infofile, char *mountfile, int ret);

void get_fullname(const char *pathname, char *fullname);
void netlink_release(void);
void netlink_init(void);

demo_sys_call_ptr_t *sys_call_table = NULL;
sys_call_t orig_connect = NULL;
sys_call_t orig_openat = NULL;
sys_call_t orig_reboot = NULL;
sys_call_t orig_execve = NULL;
sys_call_t orig_unlinkat = NULL;
sys_call_t orig_write = NULL;
sys_call_t orig_close = NULL;
sys_call_t orig_read = NULL;
sys_call_t orig_mknodat = NULL;
sys_call_t orig_finitmodule = NULL;
sys_call_t orig_deletemodule = NULL;
sys_call_t orig_socket = NULL;
sys_call_t orig_bind = NULL;
sys_call_t orig_listen = NULL;
sys_call_t orig_accept = NULL;
sys_call_t orig_sendto = NULL;
sys_call_t orig_recvfrom = NULL;
sys_call_t orig_mount = NULL;
sys_call_t orig_umount = NULL;

unsigned int level;
pte_t *pte;

int get_info_from_fd(unsigned int fd, unsigned long *ino, uid_t *uid, int *type, char *buf)
{
    struct file *file_p = NULL;
    struct inode *f_inode = NULL;
    struct fd f;
    char *buff;

    // 获取 uid
    *uid = current_uid().val; // unsigned int
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

    // get path

    buff = d_path(&file_p->f_path, buf, PATH_MAX);
    strcpy(buf, buff);

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
        ret = kernel_write(file, "\n", 1, &pos); // Add a newline character

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

asmlinkage long hacked_openat(struct pt_regs *regs)
{

    long ret;
    char buffer[PATH_MAX];
    unsigned long ino = 0;
    uid_t uid = 0;
    int index, f_type = 0;

    ret = orig_openat(regs);

    if (ret >= 0)
        get_info_from_fd(ret, &ino, &uid, &f_type, buffer);

    index = 0;
    while (buffer[index] != '\0')
    {
        if (buffer[index] == '/')
        {
            if (buffer[index + 1] == '.')
            {
                return ret;
            }
        }
        index++;
    }

    AuditOpenat(regs, buffer, ret, path);

    return ret;
}

asmlinkage long hacked_reboot(struct pt_regs *regs)
{
    long ret = 0;
    printk("FAIINFO:HERE REBOOT!!!");
    add_line_to_file("/home/faii/Desktop/a.txt", "now reboot");
    ret = orig_reboot(regs);
    return ret;
}

asmlinkage long hacked_socket(struct pt_regs *regs)
{
    long ret = 0;
    int family = (int)regs->di;
    int type = (int)regs->si;
    int protocol = (int)regs->dx;
    ret = orig_socket(regs);
    AuditSocket(regs, ret, family, type, protocol);
    return ret;
}

asmlinkage long hacked_connect(struct pt_regs *regs)
{
    long ret = 0;
    int sockfd = (int)regs->di;
    // unsigned long addr_val = regs->si;
    struct sockaddr *addr = (struct sockaddr *)regs->si;
    int addrlen = (int)regs->dx;
    // printk("hi\n");
    char buffer[14];
    // print_sockaddr(addr);
    unsigned short family = addr->sa_family;

    strncpy_from_user(buffer, addr->sa_data, 14);
    const unsigned short *port = (const unsigned short *)(buffer + 2);
    unsigned short port_tmp = *port;
    const unsigned int *ip = (const unsigned int *)(buffer + 4);
    int ip_tmp = *ip;
    // if(port_tmp){
    // if(buffer)  printk("Info:%s\n",buffer);
    // printk("Port: %hu\n", port_tmp);
    // print_ip_address(ip);}
    ret = orig_connect(regs);
    AuditConnect(regs, buffer, ret);
    return ret;
}

asmlinkage long hacked_bind(struct pt_regs *regs)
{
    long ret = 0;
    int sockfd = (int)regs->di;
    struct sockaddr *addr = (struct sockaddr *)regs->si;
    int addrlen = (int)regs->dx;
    char buffer[14];
    unsigned short family = addr->sa_family;
    strncpy_from_user(buffer, addr->sa_data, 14);
    const unsigned short *port = (const unsigned short *)(buffer + 2);
    unsigned short port_tmp = *port;
    const unsigned int *ip = (const unsigned int *)(buffer + 4);
    int ip_tmp = *ip;
    ret = orig_bind(regs);
    AuditBind(regs, buffer, ret);
    return ret;
}

asmlinkage long hacked_listen(struct pt_regs *regs)
{
    long ret = 0;
    int sockfd = (int)regs->di;
    int backlog = (int)regs->si;
    printk("arg1=%d and arg2=%d\n", sockfd, backlog);
    ret = orig_listen(regs);
    char buffer[14];
    // AuditListen(regs, buffer, ret);
    return ret;
}

asmlinkage long hacked_sendto(struct pt_regs *regs)
{
    long ret = 0;
    int sockfd = (int)regs->di;
    void *buf = (void *)regs->si;
    size_t len = (size_t)regs->dx;
    // flags is composed with bits
    int flags = (int)regs->cx;
    int addrlen = (int)regs->r9;
    // printk("arg1=%d and arg2=%d and arg3=%d and arg4=%d\n",sockfd,len,flags,addrlen);
    char buffer[14];
    ret = orig_sendto(regs);
    AuditSendto(regs, buffer, ret);
    return ret;
}

asmlinkage long hacked_recvfrom(struct pt_regs *regs)
{
    long ret = 0;
    int sockfd = (int)regs->di;
    void *buf = (void *)regs->si;
    size_t len = (size_t)regs->dx;
    // flags is composed with bits
    int flags = (int)regs->cx;
    int addrlen = (int)regs->r9;
    // printk("arg1=%d and arg2=%d and arg3=%d and arg4=%d\n",sockfd,len,flags,addrlen);
    char buffer[14];
    ret = orig_recvfrom(regs);
    AuditRecvfrom(regs, buffer, ret);
    return ret;
}

asmlinkage long hacked_execve(struct pt_regs *regs)
{
    long ret;
    char buffer_execve[PATH_MAX];
    long nbytes;
    ret = orig_execve(regs);
    nbytes = strncpy_from_user(buffer_execve, (char *)regs->di, PATH_MAX);
    AuditExecve(regs, buffer_execve, ret, path);
    return ret;
}

asmlinkage long hacked_unlinkat(struct pt_regs *regs)
{

    long ret;
    char buffer_unlink[PATH_MAX];
    long nbytes;
    nbytes = strncpy_from_user(buffer_unlink, (const char *)regs->si, PATH_MAX);
    char fullname[PATH_MAX];
    get_fullname(buffer_unlink, fullname);
    ret = orig_unlinkat(regs);
    AuditUnlinkat(regs, fullname, ret, path);
    return ret;
}

asmlinkage long hacked_write(struct pt_regs *regs)
{
    long ret;
    char buffer_write[PATH_MAX];
    long nbytes;
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;
    ret = orig_write(regs);

    if (ret >= 0)
        get_info_from_fd(regs->di, &ino, &uid, &f_type, buffer_write);
    int index = 0;
    while (buffer_write[index] != '\0')
    {
        if (buffer_write[index] == '/')
        {
            return ret;
            if (buffer_write[index + 1] == '.')
            {
                return ret;
            }
        }
        index++;
    }
    if (buffer_write[0] == ' ')
        return ret;
    if (regs->di != 1 && regs->di != 2 && regs->di != 0)
        AuditWrite(regs, buffer_write, ret, path);
    return ret;
}

asmlinkage long hacked_read(struct pt_regs *regs)
{
    unsigned long count = (unsigned long)regs->dx;

    if (count > MAX_COUNT)
    {
        count = MAX_COUNT;
    }

    void *buf = (void *)regs->si;

    char buffer[count];
    strncpy_from_user(buffer, buf, count);
    long ret;
    char buffer_read[PATH_MAX];
    long nbytes;
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;
    ret = orig_read(regs);
    if (ret >= 0)
        get_info_from_fd(regs->di, &ino, &uid, &f_type, buffer_read);
    /*
    int index = 0;
    while (buffer_read[index] != '\0') {
        if (buffer_read[index] == '/') {return ret;
            if (buffer_read[index + 1] == '.') { return ret; }
        }
        index++;
    }
    if (buffer_read[0] == ' ' )return ret;
    */
    /*
    printk("path: %s \n ", buffer_read);
    printk("fd: %lu \n ", regs->di);
    printk("info: %s \n ", buffer);
    printk("count is: %lu\n", count);
    */
    AuditRead(regs, buffer_read, ret, path);
    return ret;
}

asmlinkage long hacked_close(struct pt_regs *regs)
{
    long ret;
    char buffer_close[PATH_MAX];
    long nbytes;
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;
    ret = orig_close(regs);
    get_info_from_fd(regs->di, &ino, &uid, &f_type, buffer_close);
    if (regs->di != 1 && regs->di != 2 && regs->di != 0)
        AuditClose(regs, buffer_close, ret, path);
    return ret;
}

asmlinkage long hacked_mknodat(struct pt_regs *regs)
{
    long ret;
    char buffer_mknodat[PATH_MAX];
    long nbytes;
    ret = orig_mknodat(regs);
    nbytes = strncpy_from_user(buffer_mknodat, (const char *)regs->si, PATH_MAX);
    AuditMknodat(regs, buffer_mknodat, ret, path);
    return ret;
}

asmlinkage long hacked_finitmodule(struct pt_regs *regs)
{
    long ret;
    char buffer[PATH_MAX];
    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;

    get_info_from_fd(regs->di, &ino, &uid, &f_type, buffer);
    ret = orig_finitmodule(regs);

    if (strlen(buffer) > 0)
    {
        printk("ModuleInstalled:%s\n", buffer);
        Auditfinitmodule(buffer, ret);
    }

    return ret;
}

asmlinkage long hacked_deletemodule(struct pt_regs *regs)
{
    long ret;
    char buffer[PATH_MAX];

    strncpy_from_user(buffer, (char *)regs->di, PATH_MAX);
    ret = orig_deletemodule(regs);

    if (strlen(buffer) > 0)
    {
        printk("ModuleDeleted:%s\n", buffer);
        Auditdeletemodule(buffer, ret);
    }
    else
    {
        printk(KERN_ERR "ModuleDeletedFailedtoResolve\n");
    }

    return ret;
}

asmlinkage long hacked_mount(struct pt_regs *regs)
{
    long ret = 0;
    char source[PATH_MAX], filesystemtype[PATH_MAX], target[PATH_MAX];

    strncpy_from_user(source, (char *)regs->di, PATH_MAX);
    strncpy_from_user(filesystemtype, (char *)regs->dx, PATH_MAX);
    strncpy_from_user(target, (char *)regs->si, PATH_MAX);

    ret = orig_mount(regs);

    if (ret == 0)
    {
        printk("Mount:%s|%s|%s|%ld\n", source, filesystemtype, target, ret);
        Auditmount(source, filesystemtype, target, ret);
    }
    else
    {
        printk(KERN_ERR "Mount Failed to Resolve\n");
    }

    return ret;
}

asmlinkage long hacked_umount(struct pt_regs *regs)
{
    long ret = 0;
    char infofile[PATH_MAX], mountfile[PATH_MAX];

    unsigned long ino = 0;
    uid_t uid = 0;
    int f_type = 0;

    get_info_from_fd(regs->si, &ino, &uid, &f_type, infofile);
    strncpy_from_user(mountfile, (char *)regs->di, PATH_MAX);
    ret = orig_umount(regs);

    printk("Umount:%ld|%s|%s\n", ret, infofile, mountfile);

    Auditumount(infofile, mountfile, ret);
    return ret;
}

static int __init audit_init(void)
{

    sys_call_table = get_syscall_table();
    orig_openat = (sys_call_t)sys_call_table[__NR_openat];
    orig_reboot = (sys_call_t)sys_call_table[__NR_reboot];
    orig_connect = (sys_call_t)sys_call_table[__NR_connect];
    orig_unlinkat = (sys_call_t)sys_call_table[__NR_unlinkat];
    orig_read = (sys_call_t)sys_call_table[__NR_read];
    orig_write = (sys_call_t)sys_call_table[__NR_write];
    orig_close = (sys_call_t)sys_call_table[__NR_close];
    orig_mknodat = (sys_call_t)sys_call_table[__NR_mknodat];
    orig_execve = (sys_call_t)sys_call_table[__NR_execve];
    orig_finitmodule = (sys_call_t)sys_call_table[__NR_finit_module];
    orig_deletemodule = (sys_call_t)sys_call_table[__NR_delete_module];
    orig_socket = (sys_call_t)sys_call_table[__NR_socket];
    orig_bind = (sys_call_t)sys_call_table[__NR_bind];
    orig_sendto = (sys_call_t)sys_call_table[__NR_sendto];
    orig_recvfrom = (sys_call_t)sys_call_table[__NR_recvfrom];
    orig_mount = (sys_call_t)sys_call_table[__NR_mount];
    orig_umount = (sys_call_t)sys_call_table[__NR_umount2];
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
    sys_call_table[__NR_read] = (demo_sys_call_ptr_t)hacked_read;
    sys_call_table[__NR_write] = (demo_sys_call_ptr_t)hacked_write;
    sys_call_table[__NR_close] = (demo_sys_call_ptr_t)hacked_close;
    sys_call_table[__NR_mknodat] = (demo_sys_call_ptr_t)hacked_mknodat;
    sys_call_table[__NR_bind] = (demo_sys_call_ptr_t)hacked_bind;
    sys_call_table[__NR_listen] = (demo_sys_call_ptr_t)hacked_listen;
    sys_call_table[__NR_sendto] = (demo_sys_call_ptr_t)hacked_sendto;
    sys_call_table[__NR_recvfrom] = (demo_sys_call_ptr_t)hacked_recvfrom;
    sys_call_table[__NR_mount] = (demo_sys_call_ptr_t)hacked_mount;
    sys_call_table[__NR_umount2] = (demo_sys_call_ptr_t)hacked_umount;
    set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
    netlink_init();
    return 0;
}

static void __exit audit_exit(void)
{
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
    sys_call_table[__NR_read] = (demo_sys_call_ptr_t)orig_read;
    sys_call_table[__NR_write] = (demo_sys_call_ptr_t)orig_write;
    sys_call_table[__NR_close] = (demo_sys_call_ptr_t)orig_close;
    sys_call_table[__NR_mknodat] = (demo_sys_call_ptr_t)orig_mknodat;
    sys_call_table[__NR_bind] = (demo_sys_call_ptr_t)orig_bind;
    sys_call_table[__NR_listen] = (demo_sys_call_ptr_t)orig_listen;
    sys_call_table[__NR_sendto] = (demo_sys_call_ptr_t)orig_sendto;
    sys_call_table[__NR_recvfrom] = (demo_sys_call_ptr_t)orig_recvfrom;
    sys_call_table[__NR_mount] = (demo_sys_call_ptr_t)orig_mount;
    sys_call_table[__NR_umount2] = (demo_sys_call_ptr_t)orig_umount;
    set_pte_atomic(pte, pte_clear_flags(*pte, _PAGE_RW));
    netlink_release();
}

module_init(audit_init);
module_exit(audit_exit);
