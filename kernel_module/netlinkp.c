#include <linux/string.h>
#include <linux/mm.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>
#include <linux/limits.h>
#include <linux/namei.h>

#define TASK_COMM_LEN 16
#define NETLINK_TEST 29
#define AUDITPATH "/home/test/Desktop"
#define MAX_LENGTH 256

static u32 pid = 0;
static struct sock* nl_sk = NULL;

//发送netlink消息message
int netlink_sendmsg(const void* buffer, unsigned int size)
{

    struct sk_buff* skb;
    struct nlmsghdr* nlh;
    int len = NLMSG_SPACE(1200);
    // 检查参数的有效性
    if ((!buffer) || (!nl_sk) || (pid == 0)) 	return 1;
    skb = alloc_skb(len, GFP_ATOMIC); 	//分配一个新的sk_buffer,用于存储消息
    if (!skb) {
        printk(KERN_ERR "net_link: allocat_skb failed.\n");
        return 1;
    }
    // 使用nlmsg_put函数创建一个nlmsghdr头部，并将消息复制到NLMSG_DATA(nlh)指定的位置。
    nlh = nlmsg_put(skb, 0, 0, 0, 1200, 0);
    NETLINK_CB(skb).creds.pid = 0;      /* from kernel */
    //下面必须手动设置字符串结束标志\0，否则用户程序可能出现接收乱码
    memcpy(NLMSG_DATA(nlh), buffer, size);
    //使用netlink单播函数发送消息
    int tmp = netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
    if (tmp < 0) {
        //如果发送失败，则打印警告并退出函数
        printk(KERN_ERR "net_link: can not unicast skb with %d \n", tmp);
        return 1;
    }
    return 0;
}

// 用于获取完整的文件路径名。
void get_fullname(const char* pathname, char* fullname) {
    struct path mypath;
    struct dentry* parent_dentry = NULL;
    char buf[MAX_LENGTH];
    int code = kern_path(pathname, LOOKUP_FOLLOW, &mypath), index = 0, relative = 0;

    if (code != 0) {
        //printk(KERN_ERR "Failed to get absolute path.code:%d\npath:%s",code,pathname);
        return;
    }
    parent_dentry = mypath.dentry->d_parent;

    while (pathname[index] != '\0') {
        if (pathname[index] == '/') {
            if (pathname[index + 1] == '.') { return; }
            relative = index + 1;
        }
        index++;
    }
    pathname += relative;

    if (*(parent_dentry->d_name.name) == '/') {
        strcpy(fullname, pathname);
        return;
    }

    for (;;) {
        if (strcmp(parent_dentry->d_name.name, "/") == 0)
            buf[0] = '\0';//reach the root dentry.
        else
            strcpy(buf, parent_dentry->d_name.name);
        strcat(buf, "/");
        strcat(buf, fullname);
        strcpy(fullname, buf);

        if ((parent_dentry == NULL) || (*(parent_dentry->d_name.name) == '/'))
            break;

        parent_dentry = parent_dentry->d_parent;
    }

    strcat(fullname, pathname);
    return;
}

int AuditOpenat(struct pt_regs* regs, char* pathname, int ret)
{
    //存储命令名称的commandname、完整路径名的fullname、消息缓冲区的大小size
    char commandname[TASK_COMM_LEN];
    unsigned int size;
    void* buffer;
    char auditpath[PATH_MAX];
    const struct cred* cred;

    memset(auditpath, 0, PATH_MAX);

    // 预定义的审计路径AUDITPATH存储在auditpath中。
    strcpy(auditpath, AUDITPATH);

    // 如果fullname不是以auditpath开头，则返回1，表示不需要进行审计。
    if (strncmp(pathname, auditpath, strlen(auditpath)) != 0) return 1;

    printk("Info: fullname is  %s \t; Auditpath is  %s \n", pathname, AUDITPATH);

    // 然后，它将当前进程的命令名称复制到commandname中。
    // current 是一个全局变量，它指向 task_struct 结构体，该结构体包含了与当前进程相关的信息。
    strncpy(commandname, current->comm, TASK_COMM_LEN);

    //16：表示用于存储用户ID、进程ID、regs->dx 和打开文件返回值的 4 个整数的总大小。每个整数占用 4 个字节，所以总共占用 16 个字节。
    //1：表示用于存储字符串结束符 \0 的一个字节。
    size = strlen(pathname) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    // 接着，它将当前进程的用户ID、进程ID、regs->dx和打开文件的返回值存储到buffer中。
    cred = current_cred();
    *((int*)buffer) = __NR_openat;
    *((int*)buffer + 1) = cred->uid.val;   //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = 0;
    *((int*)buffer + 5) = 0;
    *((int*)buffer + 6) = ret;

    //最后，将commandname和fullname添加到buffer中，并调用netlink_sendmsg函数发送netlink消息。
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), pathname);
    netlink_sendmsg(buffer, size);
    return 0;
}


int AuditConnect(struct pt_regs* regs, char* netbuf, int ret) {

    char commandname[TASK_COMM_LEN];
    strncpy(commandname, current->comm, TASK_COMM_LEN);
    unsigned int size;
    void* buffer;
    size = strlen(netbuf) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    const unsigned short* port = (const unsigned short*)(netbuf + 2);
    unsigned short port_tmp = *port;
    int port_int = (int)port_tmp;
    const unsigned int* ip = (const unsigned int*)(netbuf + 4);
    int ip_tmp = *ip;
    const struct cred* cred;
    cred = current_cred();
    *((int*)buffer) = __NR_connect;
    *((int*)buffer + 1) = cred->uid.val;
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = port_int;   //uid
    *((int*)buffer + 4) = ip_tmp;
    *((int*)buffer + 5) = regs->di;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), netbuf);
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditBind(struct pt_regs* regs, char* netbuf, int ret) {

    char commandname[TASK_COMM_LEN];
    strncpy(commandname, current->comm, TASK_COMM_LEN);
    unsigned int size;
    void* buffer;
    size = strlen(netbuf) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    const unsigned short* port = (const unsigned short*)(netbuf + 2);
    unsigned short port_tmp = *port;
    int port_int = (int)port_tmp;
    const unsigned int* ip = (const unsigned int*)(netbuf + 4);
    int ip_tmp = *ip;
    const struct cred* cred;
    cred = current_cred();
    *((int*)buffer) = __NR_bind;
    *((int*)buffer + 1) = cred->uid.val;
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = port_int;   //uid
    *((int*)buffer + 4) = regs->di;
    *((int*)buffer + 5) = regs->dx;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), netbuf);
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditSendto(struct pt_regs* regs, char* netbuf, int ret) {

    char commandname[TASK_COMM_LEN];
    strncpy(commandname, current->comm, TASK_COMM_LEN);
    unsigned int size;
    void* buffer;
    size = strlen(netbuf) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    const unsigned short* port = (const unsigned short*)(netbuf + 2);
    unsigned short port_tmp = *port;
    int port_int = (int)port_tmp;
    const unsigned int* ip = (const unsigned int*)(netbuf + 4);
    int ip_tmp = *ip;
    const struct cred* cred;
    cred = current_cred();
    *((int*)buffer) = __NR_sendto;
    *((int*)buffer + 1) = cred->uid.val;
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->di;   //uid
    *((int*)buffer + 4) = regs->cx;
    *((int*)buffer + 5) = regs->dx;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), netbuf);
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditRecvfrom(struct pt_regs* regs, char* netbuf, int ret) {

    char commandname[TASK_COMM_LEN];
    strncpy(commandname, current->comm, TASK_COMM_LEN);
    unsigned int size;
    void* buffer;
    size = strlen(netbuf) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    const unsigned short* port = (const unsigned short*)(netbuf + 2);
    unsigned short port_tmp = *port;
    int port_int = (int)port_tmp;
    const unsigned int* ip = (const unsigned int*)(netbuf + 4);
    int ip_tmp = *ip;
    const struct cred* cred;
    cred = current_cred();
    *((int*)buffer) = __NR_recvfrom;
    *((int*)buffer + 1) = cred->uid.val;
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->di;   //uid
    *((int*)buffer + 4) = regs->si;
    *((int*)buffer + 5) = regs->dx;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), netbuf);
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditExecve(struct pt_regs* regs, char* pathname, int ret, char* path)
{

    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void* buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred* cred;
    int a = 0;int b = 0;int c = 0;

    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

    get_fullname(pathname, fullname);


    strcpy(auditpath, path);
    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;

    // printk("Info: operation:exe\t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

    strncpy(commandname, current->comm, TASK_COMM_LEN);
    size = strlen(fullname) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    cred = current_cred();
    *((int*)buffer) = __NR_execve;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = a;
    *((int*)buffer + 4) = b;
    *((int*)buffer + 5) = c;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditUnlinkat(struct pt_regs* regs, char* fullname, int ret, char* path)
{
    char commandname[TASK_COMM_LEN];
    unsigned int size;
    void* buffer;
    int a = 0;int b = 0;int c = 0;
    char auditpath[PATH_MAX];
    const struct cred* cred;
    memset(auditpath, 0, PATH_MAX);
    strcpy(auditpath, path);
    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;

    // printk("Info: operation:unlink \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

    strncpy(commandname, current->comm, TASK_COMM_LEN);

    size = strlen(fullname) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    cred = current_cred();
    *((int*)buffer) = __NR_unlinkat;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = a; // regs->dx: mode for open file
    *((int*)buffer + 4) = b;
    *((int*)buffer + 5) = c;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditRead(struct pt_regs* regs, char* pathname, int ret, char* path)
{
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void* buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred* cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);


    get_fullname(pathname, fullname);

    //printk("Info: fullname is  %s \n",fullname);


    strcpy(auditpath, path);

    // printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);
    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;


    strncpy(commandname, current->comm, TASK_COMM_LEN);

    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    *((int*)buffer) = __NR_read;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
    strcpy((char*)(5 + (int*)buffer), commandname);
    strcpy((char*)(5 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditWrite(struct pt_regs* regs, char* pathname, int ret, char* path)
{
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void* buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred* cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);
    get_fullname(pathname, fullname);

    //printk("Info: fullname is  %s \n",fullname);


    strcpy(auditpath, path);
    // printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);

    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;
    // printk("Info: operation:write \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);


    strncpy(commandname, current->comm, TASK_COMM_LEN);

    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    cred = current_cred();
    *((int*)buffer) = __NR_write;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
    strcpy((char*)(5 + (int*)buffer), commandname);
    strcpy((char*)(5 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditClose(struct pt_regs* regs, char* pathname, int ret, char* path)
{
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void* buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred* cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

    get_fullname(pathname, fullname);

    //printk("Info: fullname is  %s \n",fullname);


    strcpy(auditpath, path);

    // printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);
    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;
    // printk("Info: operation:close \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

    strncpy(commandname, current->comm, TASK_COMM_LEN);

    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    *((int*)buffer) = __NR_close;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = 0; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
    strcpy((char*)(5 + (int*)buffer), commandname);
    strcpy((char*)(5 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditMknodat(struct pt_regs* regs, char* pathname, int ret, char* path)
{
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void* buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred* cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

    get_fullname(pathname, fullname);

    //printk("Info: fullname is  %s \n",fullname);


    strcpy(auditpath, path);

    // printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);
    if (strncmp(fullname, auditpath, strlen(auditpath)) != 0) return 1;
    // printk("Info: operation:mknod \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

    strncpy(commandname, current->comm, TASK_COMM_LEN);

    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    *((int*)buffer) = __NR_mknodat;
    *((int*)buffer + 1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
    strcpy((char*)(5 + (int*)buffer), commandname);
    strcpy((char*)(5 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditfinitmodule(char* pathname, int ret)
{
    unsigned int size;
    void* buffer; // = kmalloc(size, 0);
    const struct cred* cred;
    char commandname[TASK_COMM_LEN];

    size = strlen(pathname) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    strncpy(commandname, current->comm, TASK_COMM_LEN);

    *((int*)buffer) = __NR_finit_module;
    *((int*)buffer + 1) = cred->uid.val;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = 0;
    *((int*)buffer + 4) = 0;
    *((int*)buffer + 5) = 0;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), pathname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditdeletemodule(char* modulename, int ret) {
    unsigned int size;
    void* buffer; // = kmalloc(size, 0);
    const struct cred* cred;
    char commandname[TASK_COMM_LEN];

    size = strlen(modulename) + TASK_COMM_LEN + 28 + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    strncpy(commandname, current->comm, TASK_COMM_LEN);

    *((int*)buffer) = __NR_delete_module;
    *((int*)buffer + 1) = cred->uid.val;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = 0;
    *((int*)buffer + 4) = 0;
    *((int*)buffer + 5) = 0;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), modulename);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditmount(char* source, char* filesystemtype, char* target, int ret) {
    unsigned int size;
    void* buffer; // = kmalloc(size, 0);
    const struct cred* cred;
    char commandname[TASK_COMM_LEN];

    size = strlen(source) + strlen(filesystemtype) + strlen(target) + TASK_COMM_LEN + 28 + 6;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    strncpy(commandname, current->comm, TASK_COMM_LEN);

    *((int*)buffer) = __NR_mount;
    *((int*)buffer + 1) = cred->uid.val;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = 0;
    *((int*)buffer + 4) = 0;
    *((int*)buffer + 5) = 0;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), source);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + strlen(source) / 4 + (int*)buffer)+3, filesystemtype);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + strlen(source) / 4 + strlen(filesystemtype) / 4 + (int*)buffer)+4, target);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditumount(char* infofile, char* mountfile, int ret) {
    unsigned int size;
    void* buffer; // = kmalloc(size, 0);
    const struct cred* cred;
    char commandname[TASK_COMM_LEN];

    size = strlen(mountfile) + TASK_COMM_LEN + 28 + 4;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    cred = current_cred();
    strncpy(commandname, current->comm, TASK_COMM_LEN);

    *((int*)buffer) = __NR_umount2;
    *((int*)buffer + 1) = cred->uid.val;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = 0;
    *((int*)buffer + 4) = 0;
    *((int*)buffer + 5) = 0;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), infofile);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + strlen(infofile) / 4 + (int*)buffer)+4, mountfile);

    netlink_sendmsg(buffer, size);
    return 0;

}

int AuditReboot(struct pt_regs* regs, int ret)
{
    unsigned int size;
    void* buffer;
    size = 4;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    *((int*)buffer) = __NR_reboot;
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditSocket(struct pt_regs* regs, int ret, int a, int b, int c)
{
    char commandname[TASK_COMM_LEN];
    unsigned int size;
    void* buffer;
    char fullname[PATH_MAX];
    char auditpath[PATH_MAX];
    size = strlen(fullname) + 28 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    const struct cred* cred;
    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);
    memset(buffer, 0, size);
    strcpy(auditpath, AUDITPATH);
    // // audit path --- 
    strncpy(commandname, current->comm, TASK_COMM_LEN);
    cred = current_cred();
    *((int*)buffer) = __NR_socket;
    *((int*)buffer + 1) = cred->uid.val;
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = a;
    *((int*)buffer + 4) = b;
    *((int*)buffer + 5) = c;
    *((int*)buffer + 6) = ret;
    strcpy((char*)(7 + (int*)buffer), commandname);
    strcpy((char*)(7 + TASK_COMM_LEN / 4 + (int*)buffer), fullname);
    netlink_sendmsg(buffer, size);
    return 0;
}

//netlink数据到达时的回调函数。
void nl_data_ready(struct sk_buff* __skb)
{
    struct sk_buff* skb;
    struct nlmsghdr* nlh;
    // 从给定的sk_buff中获取有效的skb。
    skb = skb_get(__skb);
    // 检查消息的长度是否大于等于NLMSG_SPACE(0)，以确保消息头的有效性。
    if (skb->len >= NLMSG_SPACE(0)) {
        nlh = nlmsg_hdr(skb);
        //	if( pid != 0 ) printk("Pid != 0 \n ");
        //	提取消息的发送进程的PID，并将其存储在pid变量中。
        pid = nlh->nlmsg_pid; /*pid of sending process */
        //printk("net_link: pid is %d, data %s:\n", pid, (char *)NLMSG_DATA(nlh));
        printk("net_link: pid is %d\n", pid);
        kfree_skb(skb);
    }
    return;
}

// 初始化netlink
// 在函数内部创建了一个netlink socket，并注册了数据到达时的回调函数。
void netlink_init(void) {
    // 创建一个netlink_kernel_cfg结构体cfg，并将其中的input成员设置为nl_data_ready函数，即设置接收数据的回调函数。
    struct netlink_kernel_cfg cfg = {
        .input = nl_data_ready,
    };
    // 使用netlink_kernel_create函数创建一个netlink socket，并将其赋值给全局变量nl_sk。
    nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST, &cfg);

    if (!nl_sk)
    {
        printk(KERN_ERR "net_link: Cannot create netlink socket.\n");
        sock_release(nl_sk->sk_socket);
    }
    else  printk("net_link: create socket ok.\n");
}

// 释放netlink资源。
void netlink_release(void) {
    if (nl_sk != NULL)
        sock_release(nl_sk->sk_socket);
}

