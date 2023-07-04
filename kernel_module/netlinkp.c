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
#define AUDITPATH "/home/test"
#define MAX_LENGTH 256

static u32 pid=0;
static struct sock *nl_sk = NULL;

//����netlink��Ϣmessage
int netlink_sendmsg(const void *buffer, unsigned int size)
{
        
    
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	int len = NLMSG_SPACE(1200);
	// ����������Ч��
	if((!buffer) || (!nl_sk) || (pid == 0)) 	return 1;
	skb = alloc_skb(len, GFP_ATOMIC); 	//����һ���µ�sk_buffer,���ڴ洢��Ϣ
	if (!skb){
		printk(KERN_ERR "net_link: allocat_skb failed.\n");
		return 1;
	}
	// ʹ��nlmsg_put��������һ��nlmsghdrͷ����������Ϣ���Ƶ�NLMSG_DATA(nlh)ָ����λ�á�
	nlh = nlmsg_put(skb,0,0,0,1200,0);
	NETLINK_CB(skb).creds.pid = 0;      /* from kernel */
	//��������ֶ������ַ���������־\0�������û�������ܳ��ֽ�������
	memcpy(NLMSG_DATA(nlh), buffer, size);
	//ʹ��netlink��������������Ϣ
	if( netlink_unicast(nl_sk, skb, pid, MSG_DONTWAIT) < 0){
	//�������ʧ�ܣ����ӡ���沢�˳�����
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
        if(pathname[index]=='/'){if (pathname[index+1] == '.') return; relative=index+1;}
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

/*
// ���ڻ�ȡ�������ļ�·������
void get_fullname(const char *pathname,char *fullname)
{
	// ��ȡ��ǰ���̵ĵ�ǰ����Ŀ¼��dentry�ṹ��ָ��parent_dentry
	struct dentry *parent_dentry = current->fs->pwd.dentry;
    char buf[MAX_LENGTH];
        // ���parent_dentry������Ϊ'/'����pathname�Ѿ�������·������ֱ�ӽ��临�Ƶ�fullname�в����ء�
	if (*(parent_dentry->d_name.name)=='/'){
	    strcpy(fullname,pathname);
	    return;
	}
	// ������ѭ����������dentry�����𲽹�������·������
	// ��ÿ��ѭ���У�������ǰ����dentry��������ӵ�buf�У�����fullname׷�ӵ�buf��ĩβ��Ȼ�󣬽�buf���Ƶ�fullname�У����¸���dentryΪ�����ĸ�����ֱ�������Ŀ¼����󣬽�pathname��ӵ�fullname��ĩβ���õ�������·������
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
*/

// int Audit_Totoal(struct pt_regs * regs,char** pass_string ,int* pass_ret)
// {
//     AuditOpenat(regs,pass_string[1],pass_ret[1]);
// }

// �¼������������ڴ����ļ��򿪲�����
// AuditOpenat(regs,buffer,ret);
int AuditOpenat(struct pt_regs * regs, char * pathname,int ret)
{
//�洢�������Ƶ�commandname������·������fullname����Ϣ�������Ĵ�Сsize
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// ����get_fullname������ȡ����·������������洢��fullname�С�

    get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// Ԥ��������·��AUDITPATH�洢��auditpath�С�
    strcpy(auditpath,AUDITPATH);
// ���fullname������auditpath��ͷ���򷵻�1����ʾ����Ҫ������ơ�
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;

    printk("Info: operation:open\t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// Ȼ��������ǰ���̵��������Ƹ��Ƶ�commandname�С�
// current ��һ��ȫ�ֱ�������ָ�� task_struct �ṹ�壬�ýṹ��������뵱ǰ������ص���Ϣ��
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16����ʾ���ڴ洢�û�ID������ID��regs->dx �ʹ��ļ�����ֵ�� 4 ���������ܴ�С��ÿ������ռ�� 4 ���ֽڣ������ܹ�ռ�� 16 ���ֽڡ�
//1����ʾ���ڴ洢�ַ��������� \0 ��һ���ֽڡ�
    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// ���ţ�������ǰ���̵��û�ID������ID��regs->dx�ʹ��ļ��ķ���ֵ�洢��buffer�С�
    cred = current_cred();
    *((int*)buffer)=__NR_openat;
    *((int*)buffer+1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//��󣬽�commandname��fullname��ӵ�buffer�У�������netlink_sendmsg��������netlink��Ϣ��
    strcpy( (char*)( 5 + (int*)buffer ), commandname);
    strcpy( (char*)( 5 + TASK_COMM_LEN/4 +(int*)buffer ), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditExecve(struct pt_regs * regs, char * pathname,int ret)
{
    
//�洢�������Ƶ�commandname������·������fullname����Ϣ�������Ĵ�Сsize
    char commandname[TASK_COMM_LEN];
    char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// ����get_fullname������ȡ����·������������洢��fullname�С�
    get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// Ԥ��������·��AUDITPATH�洢��auditpath�С�
    strcpy(auditpath,AUDITPATH);
// ���fullname������auditpath��ͷ���򷵻�1����ʾ����Ҫ������ơ�
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;

    printk("Info: operation:exe\t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// Ȼ��������ǰ���̵��������Ƹ��Ƶ�commandname�С�
// current ��һ��ȫ�ֱ�������ָ�� task_struct �ṹ�壬�ýṹ��������뵱ǰ������ص���Ϣ��
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16����ʾ���ڴ洢�û�ID������ID��regs->dx �ʹ��ļ�����ֵ�� 4 ���������ܴ�С��ÿ������ռ�� 4 ���ֽڣ������ܹ�ռ�� 16 ���ֽڡ�
//1����ʾ���ڴ洢�ַ��������� \0 ��һ���ֽڡ�
    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// ���ţ�������ǰ���̵��û�ID������ID��regs->dx�ʹ��ļ��ķ���ֵ�洢��buffer�С�
    cred = current_cred();
    *((int*)buffer)=__NR_execve;
    *((int*)buffer+1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//��󣬽�commandname��fullname��ӵ�buffer�У�������netlink_sendmsg��������netlink��Ϣ��
    strcpy( (char*)( 5 + (int*)buffer ), commandname);
    strcpy( (char*)( 5 + TASK_COMM_LEN/4 +(int*)buffer ), fullname);

    netlink_sendmsg(buffer, size);
    return 0;
}

int AuditUnlinkat(struct pt_regs * regs, char * fullname,int ret)
{
    // printk("get in audit_unlink");
//�洢�������Ƶ�commandname������·������fullname����Ϣ�������Ĵ�Сsize
    char commandname[TASK_COMM_LEN];
    // char fullname[PATH_MAX];
    unsigned int size;   // = strlen(pathname) + 32 + TASK_COMM_LEN;
    void * buffer; // = kmalloc(size, 0);
    char auditpath[PATH_MAX];
    const struct cred *cred;


    // memset(fullname, 0, PATH_MAX);
    memset(auditpath, 0, PATH_MAX);

// ����get_fullname������ȡ����·������������洢��fullname�С�
    // get_fullname(pathname,fullname);

    //printk("Info: fullname is  %s \n",fullname);

// Ԥ��������·��AUDITPATH�洢��auditpath�С�
    strcpy(auditpath,AUDITPATH);
// ���fullname������auditpath��ͷ���򷵻�1����ʾ����Ҫ������ơ�
// printk("pathname: %s fullname: %s auditpath: %s", pathname, fullname, auditpath);
    if (strncmp(fullname,auditpath,strlen(auditpath)) != 0) return 1;
    printk("Info: operation:unlink \t fullname is  %s \t; Auditpath is  %s \n",fullname,AUDITPATH);

// Ȼ��������ǰ���̵��������Ƹ��Ƶ�commandname�С�
// current ��һ��ȫ�ֱ�������ָ�� task_struct �ṹ�壬�ýṹ��������뵱ǰ������ص���Ϣ��
    strncpy(commandname,current->comm,TASK_COMM_LEN);
//16����ʾ���ڴ洢�û�ID������ID��regs->dx �ʹ��ļ�����ֵ�� 4 ���������ܴ�С��ÿ������ռ�� 4 ���ֽڣ������ܹ�ռ�� 16 ���ֽڡ�
//1����ʾ���ڴ洢�ַ��������� \0 ��һ���ֽڡ�
    size = strlen(fullname) + 20 + TASK_COMM_LEN + 1;
    buffer = kmalloc(size, 0);
    memset(buffer, 0, size);
// ���ţ�������ǰ���̵��û�ID������ID��regs->dx�ʹ��ļ��ķ���ֵ�洢��buffer�С�
    cred = current_cred();
    *((int*)buffer)=__NR_unlinkat;
    *((int*)buffer+1) = cred->uid.val; ;  //uid
    *((int*)buffer + 2) = current->pid;
    *((int*)buffer + 3) = regs->dx; // regs->dx: mode for open file
    *((int*)buffer + 4) = ret;
//��󣬽�commandname��fullname��ӵ�buffer�У�������netlink_sendmsg��������netlink��Ϣ��
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

//netlink���ݵ���ʱ�Ļص�������
void nl_data_ready(struct sk_buff *__skb)
 {
    struct sk_buff *skb;
    struct nlmsghdr *nlh;
    // �Ӹ�����sk_buff�л�ȡ��Ч��skb��
    skb = skb_get (__skb);
// �����Ϣ�ĳ����Ƿ���ڵ���NLMSG_SPACE(0)����ȷ����Ϣͷ����Ч�ԡ�
    if (skb->len >= NLMSG_SPACE(0)) {
	nlh = nlmsg_hdr(skb);
//	if( pid != 0 ) printk("Pid != 0 \n ");
//	��ȡ��Ϣ�ķ��ͽ��̵�PID��������洢��pid�����С�
	pid = nlh->nlmsg_pid; /*pid of sending process */
	//printk("net_link: pid is %d, data %s:\n", pid, (char *)NLMSG_DATA(nlh));
	printk("net_link: pid is %d\n", pid);
	kfree_skb(skb);
    }
    return;
}


// ��ʼ��netlink
// �ں����ڲ�������һ��netlink socket����ע�������ݵ���ʱ�Ļص�������
void netlink_init(void) {
// ����һ��netlink_kernel_cfg�ṹ��cfg���������е�input��Ա����Ϊnl_data_ready�����������ý������ݵĻص�������
    struct netlink_kernel_cfg cfg = {
        .input = nl_data_ready,
    };
// ʹ��netlink_kernel_create��������һ��netlink socket�������丳ֵ��ȫ�ֱ���nl_sk��
    nl_sk=netlink_kernel_create(&init_net,NETLINK_TEST, &cfg);

    if (!nl_sk)
    {
		printk(KERN_ERR "net_link: Cannot create netlink socket.\n");
		sock_release(nl_sk->sk_socket);
    }
    else  printk("net_link: create socket ok.\n");
}

// �ͷ�netlink��Դ��
void netlink_release(void) {
    if (nl_sk != NULL)
 		sock_release(nl_sk->sk_socket);
}
