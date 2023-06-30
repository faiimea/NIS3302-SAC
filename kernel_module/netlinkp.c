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
#define AUDITPATH "/home/innovation"
#define MAX_LENGTH 256
static u32 pid=7425;
static struct sock *nl_sk = NULL;

//发送netlink消息message
int netlink_sendmsg(const void *buffer, unsigned int size)
{
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(1200);
	if((!buffer) || (!nl_sk) || (pid == 0)){
		printk(KERN_ERR "net_link: something failed.%d,%d,%d\n",(!buffer),(!nl_sk),(pid==0));
		return 1;
	} 	
	skb = alloc_skb(len, GFP_ATOMIC); 	//分配一个新的sk_buffer
	if (!skb){
		printk(KERN_ERR "net_link: allocat_skb failed.\n");
		return 1;
	}
	nlh = nlmsg_put(skb,0,0,0,1200,0);
	NETLINK_CB(skb).creds.pid = 0;      /* from kernel */
	//下面必须手动设置字符串结束标志\0，否则用户程序可能出现接收乱码
	//edit this part to determine where the file path ends and add '\0'
	memcpy(NLMSG_DATA(nlh), buffer, size);
	//使用netlink单播函数发送消息
	if( netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT) < 0){
	//如果发送失败，则打印警告并退出函数
		printk(KERN_ERR "net_link: can not unicast skb \n");
		return 1;
	}
	return 0;
}


void get_fullname(const char *pathname, char *fullname) {
    struct path mypath;
    struct dentry *parent_dentry = NULL;
    char buf[MAX_LENGTH];
    int code=kern_path(pathname, LOOKUP_FOLLOW, &mypath),index=0,relative=0;

    if (code != 0) {
        //printk(KERN_ERR "Failed to get absolute path.code:%d\npath:%s",code,pathname);
        return;
    }
    parent_dentry= mypath.dentry->d_parent;

    while(pathname[index]!='\0'){
        if(pathname[index]=='/'){relative=index+1;}
        index++;
    }
    pathname+=relative;

    if (*(parent_dentry->d_name.name)=='/'){
        strcpy(fullname,pathname);
        return;
    }

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

int AuditOpenat(void)
{
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);


    size = strlen(fullname) + 16 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);

    *((int*)buffer) = 9;   
    *((int*)buffer + 1) = 9;
    *((int*)buffer + 2) = 9; // regs->dx: mode for open file
    *((int*)buffer + 3) = 9;

    netlink_sendmsg(buffer, size);
    return 0;
}


void nl_data_ready(struct sk_buff *__skb)
 {
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    skb = skb_get (__skb);

    if (skb->len >= NLMSG_SPACE(0)) {
	nlh = nlmsg_hdr(skb);
//	if( pid != 0 ) printk("Pid != 0 \n ");
	pid = nlh->nlmsg_pid; /*pid of sending process */
	//printk("net_link: pid is %d, data %s:\n", pid, (char *)NLMSG_DATA(nlh));
	printk("net_link: pid is %d\n", pid);
	kfree_skb(skb);
    }
    return;
}



int netlink_init(void) {
    struct netlink_kernel_cfg cfg = {
        .input = nl_data_ready,
    };

    nl_sk=netlink_kernel_create(&init_net,NETLINK_TEST, &cfg);

    if (!nl_sk)
    {
		printk(KERN_ERR "net_link: Cannot create netlink socket.\n");
		sock_release(nl_sk->sk_socket);
		return 0;
    }
    else  printk("net_link: create socket ok.\n");
    return 1;
}


void netlink_release(void) {
    if (nl_sk != NULL)
 		sock_release(nl_sk->sk_socket);
}
