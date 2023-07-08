#include <linux/string.h>
#include <linux/mm.h>
#include <net/sock.h>
#include <net/netlink.h>
#include <linux/sched.h>
#include <linux/fs_struct.h>
#include <linux/limits.h>

#define TASK_COMM_LEN 16
#define NETLINK_TEST 29
#define AUDITPATH "/home/faii/Desktop/"
#define MAX_LENGTH 256

static u32 pid=0;
static struct sock *nl_sk = NULL;

//发送netlink消息message
int netlink_sendmsg(const void *buffer, unsigned int size)
{
    
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(1200);
	// 检查参数的有效性
	if((!buffer) || (!nl_sk) || (pid == 0)) 	return 1;
	skb = alloc_skb(len, GFP_ATOMIC); 	//分配一个新的sk_buffer,用于存储消息
	if (!skb){
		printk(KERN_ERR "net_link: allocat_skb failed.\n");
		return 1;
	}
	// 使用nlmsg_put函数创建一个nlmsghdr头部，并将消息复制到NLMSG_DATA(nlh)指定的位置。
	nlh = nlmsg_put(skb,0,0,0,1200,0);
	NETLINK_CB(skb).creds.pid = 0;      /* from kernel */
	//下面必须手动设置字符串结束标志\0，否则用户程序可能出现接收乱码
	memcpy(NLMSG_DATA(nlh), buffer, size);
	//使用netlink单播函数发送消息
    int tmp=netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT);
	if( tmp < 0){
	//如果发送失败，则打印警告并退出函数
		printk(KERN_ERR "net_link: can not unicast skb with %d \n",tmp);
		return 1;
	}
	return 0;
}

// 用于获取完整的文件路径名。
void get_fullname(const char *pathname,char *fullname)
{
	// 获取当前进程的当前工作目录的dentry结构体指针parent_dentry
	struct dentry *parent_dentry = current->fs->pwd.dentry;
    char buf[MAX_LENGTH];


        // 如果parent_dentry的名称为'/'，则pathname已经是完整路径名，直接将其复制到fullname中并返回。
	if (*(parent_dentry->d_name.name)=='/'){
	    strcpy(fullname,pathname);
	    return;
	}

	// 否则，它循环遍历父级dentry，并逐步构建完整路径名。
	// 在每次循环中，它将当前父级dentry的名称添加到buf中，并将fullname追加到buf的末尾。然后，将buf复制到fullname中，更新父级dentry为父级的父级，直到到达根目录。最后，将pathname添加到fullname的末尾，得到完整的路径名。
	for(;;){
	    if (strcmp(parent_dentry->d_name.name,"/")==0)
            buf[0]='\0';//reach the root dentry.
	    else
	        strcpy(buf,parent_dentry->d_name.name);
        strcat(buf,"/");
        strcat(buf,fullname);
        strcpy(fullname,buf);

        if ((parent_dentry == NULL) || (*(parent_dentry->d_name.name)=='/'))
            break;

        parent_dentry = parent_dentry->d_parent;
	}

	strcat(fullname,pathname);

	return;
}

int AuditOpenat(struct pt_regs * regs, char * pathname,int ret)
{
    int flag=1;
//存储命令名称的commandname、完整路径名的fullname、消息缓冲区的大小size
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// 调用get_fullname函数获取完整路径名，并将其存储在fullname中。
    get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// 预定义的审计路径AUDITPATH存储在auditpath中。
    strcpy(auditpath,AUDITPATH);
// 如果fullname不是以auditpath开头，则返回1，表示不需要进行审计。
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;

    printk("Info: fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// 然后，它将当前进程的命令名称复制到commandname中。
// current 是一个全局变量，它指向 task_struct 结构体，该结构体包含了与当前进程相关的信息。
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16：表示用于存储用户ID、进程ID、regs->dx 和打开文件返回值的 4 个整数的总大小。每个整数占用 4 个字节，所以总共占用 16 个字节。
//1：表示用于存储字符串结束符 \0 的一个字节。
    size = strlen(fullname) +4 + 16 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// 接着，它将当前进程的用户ID、进程ID、regs->dx和打开文件的返回值存储到buffer中。
    cred = current_cred();
    *((int*)buffer) = __NR_openat;
    *((int*)buffer+1) = cred->uid.val;   //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//最后，将commandname和fullname添加到buffer中，并调用netlink_sendmsg函数发送netlink消息。
    strcpy( (char*)( 5 + (int*)buffer ), commandname);
    strcpy( (char*)( 5 + TASK_COMM_LEN/4 +(int*)buffer ), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditReboot(struct pt_regs * regs,int ret)
{
    unsigned int size;   
    void * buffer; 
    size = 4;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    *((int*)buffer) = 114;
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditConnect(struct pt_regs * regs,char * netbuf,int ret){
    unsigned int size;
    void * buffer;
    size=54;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    const unsigned short *port = (const unsigned short *)(netbuf + 2);
    unsigned short port_tmp=*port;
    int port_int=(int)port_tmp;
    const unsigned int *ip = (const unsigned int *)(netbuf + 4);
    int ip_tmp=*ip;
    int flag=__NR_connect;
    *((int*)buffer) = flag;
    *((int*)buffer+1) = port_int;   //uid
    printk("Port: %d\n", port_int);
    *((int*)buffer + 2) = ip_tmp;
    printk("ip: %d\n", ip_tmp);
    *((int*)buffer + 3) = regs->di;
    *((int*)buffer + 4) = regs->dx;
    strcpy( (char*)( 5 + (int*)buffer ), netbuf);
    printk("netbuf:%s\n",netbuf);
    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditExecve(struct pt_regs * regs, char * pathname,int ret)
{
    
//存储命令名称的commandname、完整路径名的fullname、消息缓冲区的大小size
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// 调用get_fullname函数获取完整路径名，并将其存储在fullname中。
    get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// 预定义的审计路径AUDITPATH存储在auditpath中。
    strcpy(auditpath,AUDITPATH);
// 如果fullname不是以auditpath开头，则返回1，表示不需要进行审计。
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;

    printk("Info: operation:exe\t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// 然后，它将当前进程的命令名称复制到commandname中。
// current 是一个全局变量，它指向 task_struct 结构体，该结构体包含了与当前进程相关的信息。
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16：表示用于存储用户ID、进程ID、regs->dx 和打开文件返回值的 4 个整数的总大小。每个整数占用 4 个字节，所以总共占用 16 个字节。
//1：表示用于存储字符串结束符 \0 的一个字节。
    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// 接着，它将当前进程的用户ID、进程ID、regs->dx和打开文件的返回值存储到buffer中。
    cred = current_cred();
    *((int*)buffer)=__NR_execve;
    *((int*)buffer+1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//最后，将commandname和fullname添加到buffer中，并调用netlink_sendmsg函数发送netlink消息。
    strcpy( (char*)( 5 + (int*)buffer ), commandname);
    strcpy( (char*)( 5 + TASK_COMM_LEN/4 +(int*)buffer ), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditUnlinkat(struct pt_regs * regs, char * fullname,int ret)
{
    // printk("get in audit_unlink");
//存储命令名称的commandname、完整路径名的fullname、消息缓冲区的大小size
    char commandname[TASK_COMM_LEN];
    // char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    // memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// 调用get_fullname函数获取完整路径名，并将其存储在fullname中。
    // get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// 预定义的审计路径AUDITPATH存储在auditpath中。
    strcpy(auditpath,AUDITPATH);
// 如果fullname不是以auditpath开头，则返回1，表示不需要进行审计。
// printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;
    printk("Info: operation:unlink \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// 然后，它将当前进程的命令名称复制到commandname中。
// current 是一个全局变量，它指向 task_struct 结构体，该结构体包含了与当前进程相关的信息。
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16：表示用于存储用户ID、进程ID、regs->dx 和打开文件返回值的 4 个整数的总大小。每个整数占用 4 个字节，所以总共占用 16 个字节。
//1：表示用于存储字符串结束符 \0 的一个字节。
    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// 接着，它将当前进程的用户ID、进程ID、regs->dx和打开文件的返回值存储到buffer中。
    cred = current_cred();
    *((int*)buffer)=__NR_unlinkat;
    *((int*)buffer+1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//最后，将commandname和fullname添加到buffer中，并调用netlink_sendmsg函数发送netlink消息。
    strcpy( (char*)( 5 + (int*)buffer ), commandname);
    strcpy( (char*)( 5 + TASK_COMM_LEN/4 +(int*)buffer ), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditfinitmodule(char * pathname)
{
    unsigned int size; 
    void * buffer; // = kmalloc(size, 0);

    size = strlen(pathname) + 4 + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    *((int*)buffer)=__NR_finit_module;
    strcpy( (char*)( 1 + (int*)buffer ), pathname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int Auditdeletemodule(char * modulename){
    unsigned int size; 
    void * buffer; // = kmalloc(size, 0);

    size = strlen(modulename) + 4 + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    *((int*)buffer)=__NR_delete_module;
    strcpy( (char*)( 1 + (int*)buffer ), modulename);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditReboot(struct pt_regs * regs,int ret)
{
    unsigned int size;   
    void * buffer; 
    size = 4;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
    *((int*)buffer) = 114;
    netlink_sendmsg(buffer, size);
    return 0;
}

//netlink数据到达时的回调函数。
void nl_data_ready(struct sk_buff *__skb)
 {
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    // 从给定的sk_buff中获取有效的skb。
    skb = skb_get (__skb);
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
    nl_sk=netlink_kernel_create(&init_net,NETLINK_TEST, &cfg);

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
