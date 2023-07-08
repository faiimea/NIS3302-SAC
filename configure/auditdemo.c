#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <linux/unistd.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <asm/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pwd.h>

#define TM_FMT "%Y-%m-%d %H:%M:%S"

#define NETLINK_TEST 29
#define MAX_PAYLOAD 1024  /* maximum payload size*/
int sock_fd;
struct msghdr msg;
struct nlmsghdr *nlh = NULL;
struct sockaddr_nl src_addr, dest_addr;
struct iovec iov;

FILE *logfile;

void Log_Open(char *commandname,int uid, int pid, char *file_path, int flags,int ret)
{
	char logtime[64];
	char username[32];
	struct passwd *pwinfo;
	char openresult[10];
	char opentype[16];
	if (ret > 0) strcpy(openresult,"success");
	else strcpy(openresult,"failed");
	if (flags & O_RDONLY ) strcpy(opentype, "Read");
	else if (flags & O_WRONLY ) strcpy(opentype, "Write");
	else if (flags & O_RDWR ) strcpy(opentype, "Read/Write");
	else strcpy(opentype,"other");

	time_t t=time(0);
	if (logfile == NULL)	return;
	pwinfo = getpwuid(uid);
	strcpy(username,pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t) );
	fprintf(logfile,"%s(%d) %s(%d) %s \"%s\" %s %s\n",username,uid,commandname,pid,logtime,file_path,opentype, openresult);
	printf("%s(%d) %s(%d) %s \"%s\" %s %s\n",username,uid,commandname,pid,logtime,file_path,opentype, openresult);
}

void Log_Connect(int port_int, int ip_int, const char *netbuf, int ret) {
    char logtime[64];
    char username[32];
    struct passwd *pwinfo;
    char connectresult[10];
    char ip[INET_ADDRSTRLEN];
    int port;
    
    if (ret == 0)
        strcpy(connectresult, "success");
    else
        strcpy(connectresult, "failed");

    unsigned char bytes[4];
    bytes[0] = ip_int & 0xFF;
    bytes[1] = (ip_int >> 8) & 0xFF;
    bytes[2] = (ip_int >> 16) & 0xFF;
    bytes[3] = (ip_int >> 24) & 0xFF;

	printf("ip=%s\n",bytes);
    snprintf(ip, 4, "%u.%u.%u.%u", bytes[0], bytes[1], bytes[2], bytes[3]);

    time_t t = time(0);
    if (logfile == NULL)
        return;
    //pwinfo = getpwuid(uid);
    //strcpy(username, pwinfo->pw_name);

    strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
    fprintf(logfile, "%s() %s() %s %s:%d %s\n", username, "connect", logtime, ip, port, connectresult);
    printf("%s() %s() %s %s:%d %s\n", username, "connect", logtime, ip, port, connectresult);
}

void Log_file(int operation, char *commandname,int uid, int pid, char *file_path, int flags,int ret)
{
	char logtime[64];
	char username[32];
	struct passwd *pwinfo;
	char openresult[10];
	char opentype[16];
	if (ret >= 0) strcpy(openresult,"success");
	else strcpy(openresult,"failed");
	if (flags & O_RDONLY ) strcpy(opentype, "Read");
	else if (flags & O_WRONLY ) strcpy(opentype, "Write");
	else if (flags & O_RDWR ) strcpy(opentype, "Read/Write");
	else strcpy(opentype,"other");

	time_t t=time(0);
	if (logfile == NULL)	return;
	pwinfo = getpwuid(uid);
	strcpy(username,pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t) );
	fprintf(logfile,"%s(%d) %s(%d) %s \"%s\" %s %s\n",username,uid,commandname,pid,logtime,file_path,opentype, openresult);
	if(operation == __NR_openat) printf("openat\t");
	if(operation == __NR_unlinkat) printf("unlinkat\t");
	if(operation == __NR_execve) printf("execve\t");
	
	printf("%s(%d) %s(%d) %s \"%s\" %s %s\n",username,uid,commandname,pid,logtime,file_path,opentype, openresult);
}

// Fixed , DONOT CHANGE
// has no related to specific function
void sendpid(unsigned int pid)
{
	//Send message to initialize
	memset(&msg, 0, sizeof(msg));
	memset(&src_addr, 0, sizeof(src_addr));
	src_addr.nl_family = AF_NETLINK;
	src_addr.nl_pid = pid;  //self pid
	src_addr.nl_groups = 0;  //not in mcast groups
	bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr));

	memset(&dest_addr, 0, sizeof(dest_addr));
	dest_addr.nl_family = AF_NETLINK;
	dest_addr.nl_pid = 0;   //For Linux Kernel
	dest_addr.nl_groups = 0; //unicast

	/* Fill the netlink message header */
	nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	nlh->nlmsg_pid = pid;  /* self pid */
	nlh->nlmsg_flags = 0;
	/* Fill in the netlink message payload */
	iov.iov_base = (void *)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void *)&dest_addr;
	msg.msg_namelen = sizeof(dest_addr);
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
	//printf(" Sending message. ...\n");
	sendmsg(sock_fd, &msg, 0);
}

void killdeal_func()
{
	printf("The process is killed! \n");
	close(sock_fd);
	if (logfile != NULL)
		fclose(logfile);
	if (nlh != NULL)
	 	free(nlh);
	exit(0);
}

void PreLog_Open(struct nlmsghdr *nlh) {
    unsigned int uid, pid, flags, ret;
    char *file_path;
    char *commandname;

    uid = *(1+(unsigned int *)NLMSG_DATA(nlh));
    pid = *(2 + (int *)NLMSG_DATA(nlh));
    flags = *(3 + (int *)NLMSG_DATA(nlh));
    ret = *(4 + (int *)NLMSG_DATA(nlh));
    commandname = (char *)(5 + (int *)NLMSG_DATA(nlh));
    file_path = (char *)(5 + 16 / 4 + (int *)NLMSG_DATA(nlh));

    Log_Open(commandname, uid, pid, file_path, flags, ret);
}

void print_ip_address(unsigned int ip)
{
    struct in_addr addr;
    addr.s_addr = ip;

    // 使用 inet_ntoa 函数将整数 IP 地址转换为可读的字符串
    char *ip_str = inet_ntoa(addr);

    // 打印 IP 地址
    printf("IP Address: %s\n", ip_str);
}

void PreLog_Connect(struct nlmsghdr *nlh) {
    //int port,ip,ret;printf("IP Address: %d\n", ip);

    char *netbuf;

    // port= *(1 + (int *)NLMSG_DATA(nlh));
    // ip = *(2 + (int *)NLMSG_DATA(nlh));
    //ret = *(4 + (int *)NLMSG_DATA(nlh));
    netbuf = (char *)(5 + (int *)NLMSG_DATA(nlh));
	struct sockaddr_in *addr_in = (struct sockaddr_in *)netbuf;

    // 提取 IP 和端口信息
    unsigned int ip = addr_in->sin_addr.s_addr;
    unsigned short port = addr_in->sin_port;

    // 打印 IP 和端口信息
	if(port){
    print_ip_address(ip);
    printf("Port: %hu\n", ntohs(port));}

    //Log_Connect(port, ip, netbuf, ret);
}

void PreLog_File(struct nlmsghdr *nlh) {
    unsigned int uid, pid, flags, ret, operation;
    char *file_path;
    char *commandname;
    operation = *((unsigned int *)NLMSG_DATA(nlh));
    
    uid = *(1+(unsigned int *)NLMSG_DATA(nlh));
    pid = *(2 + (int *)NLMSG_DATA(nlh));
    flags = *(3 + (int *)NLMSG_DATA(nlh));
    ret = *(4 + (int *)NLMSG_DATA(nlh));
    commandname = (char *)(5 + (int *)NLMSG_DATA(nlh));
    file_path = (char *)(5 + 16 / 4 + (int *)NLMSG_DATA(nlh));

    Log_file(operation, commandname, uid, pid, file_path, flags, ret);
}


int main(int argc, char *argv[]){
	char buff[110];
	//void killdeal_func();
	char logpath[32];
	if (argc == 1) strcpy(logpath,"./log");
	else if (argc == 2) strncpy(logpath, argv[1],32);
	else {
		printf("commandline parameters error! please check and try it! \n");
		exit(1);
	}


	signal(SIGTERM,killdeal_func);
	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

    sendpid(getpid());

	/* open the log file at the begining of daemon, in case of this operation causes deadlock */
	logfile=fopen(logpath, "w+");
	if (logfile == NULL) {
		printf("Waring: can not create log file\n");
		exit(1);
	}
	//Loop to get message
	while(1){	//Read message from kernel

		recvmsg(sock_fd, &msg, 0);
		switch(*((unsigned int *)NLMSG_DATA(nlh))){
			case __NR_openat:
				PreLog_Open(nlh);
				break;
			case __NR_execve:
			case __NR_unlinkat:
				PreLog_File(nlh);
				break;
			case __NR_connect:
				PreLog_Connect(nlh);
				break;
			default:
				printf("Error\n");
				break;
		}
		
    }
	close(sock_fd);
	free(nlh);
	fclose(logfile);
	return 0;
}


