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
struct nlmsghdr* nlh = NULL;
struct sockaddr_nl src_addr, dest_addr;
struct iovec iov;

FILE* logfile;



void Log_file(int operation, char* commandname, int uid, int pid, char* file_path, int arg1, int arg2, int arg3, int ret)
{
	char audit_type[32];

	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char openresult[10];
	char opentype[16];
	if (ret >= 0) strcpy(openresult, "success");
	else strcpy(openresult, "failed");
	if (arg1 & O_RDONLY) strcpy(opentype, "Read");
	else if (arg1 & O_WRONLY) strcpy(opentype, "Write");
	else if (arg1 & O_RDWR) strcpy(opentype, "Read/Write");
	else strcpy(opentype, "other");

	time_t t = time(0);
	if (logfile == NULL)	return;
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
	fprintf(logfile, "%s:%s(%d) %s(%d) %s \"%s\" %s %s\n", audit_type, username, uid, commandname, pid, logtime, file_path, opentype, openresult);
	if (operation == __NR_openat) { strcpy(audit_type, "openat"); }
	if (operation == __NR_unlinkat) { strcpy(audit_type, "unlinkat"); }
	if (operation == __NR_execve) { strcpy(audit_type, "execve"); }
	if (operation == __NR_read) { strcpy(audit_type, "read"); }
	if (operation == __NR_close) { strcpy(audit_type, "close"); }
	if (operation == __NR_write) { strcpy(audit_type, "write"); }
	if (operation == __NR_mknodat) { strcpy(audit_type, "mknodat"); }
	if (operation == __NR_openat) { strcpy(audit_type, "openat"); }

	printf("%s:%s(%d) %s(%d) %s \"%s\" %s %s\n", audit_type, username, uid, commandname, pid, logtime, file_path, opentype, openresult);
}

void print_ip_address(unsigned int ip)
{
	struct in_addr addr;
	addr.s_addr = ip;

	// 使用 inet_ntoa 函数将整数 IP 地址转换为可读的字符串
	char* ip_str = inet_ntoa(addr);

	// 打印 IP 地址
	printf("IP Address: %s\n", ip_str);
}


void Log_Connect(char* commandname, int uid, int pid, char* buffer, int port_int, int ip_int, int arg3, int ret) {
	char audit_type[32];
	strcpy(audit_type, "connect");
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char connectresult[10];

	struct sockaddr_in* addr_in = (struct sockaddr_in*)buffer;

	// 提取 IP 和端口信息
	unsigned int ip = addr_in->sin_addr.s_addr;
	unsigned short port = addr_in->sin_port;

	if (ret == 0)
		strcpy(connectresult, "success");
	else
		strcpy(connectresult, "failed");

	struct in_addr addr;
	addr.s_addr = ip;

	// 使用 inet_ntoa 函数将整数 IP 地址转换为可读的字符串
	char* ip_str = inet_ntoa(addr);
	time_t t = time(0);
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
	fprintf(logfile, "%s :%s(%d) %s(%d) %s  %s:%d %s\n", audit_type, username, uid, commandname, pid, logtime, ip_str, port, connectresult);
	printf("%s:%s(%d) %s(%d) %s ip= %s:port= %d %s\n", audit_type, username, uid, commandname, pid, logtime, ip_str, port, connectresult);
}

void Log_Bind(char* commandname, int uid, int pid, char* buffer, int arg1, int arg2, int arg3, int ret) {
	char audit_type[32];
	strcpy(audit_type, "bind");
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char connectresult[10];
	int sockfd = arg2;
	int addrlen = arg3;
	if (ret == 0)
		strcpy(connectresult, "success");
	else
		strcpy(connectresult, "failed");

	struct in_addr addr;
	time_t t = time(0);
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
	fprintf(logfile, "%s :%s(%d) %s(%d) %s  %d:%d %s\n", audit_type, username, uid, commandname, pid, logtime, sockfd, addrlen, connectresult);
	printf("%s:%s(%d) %s(%d) %s sockfd= %d:addrlen= %d %s\n", audit_type, username, uid, commandname, pid, logtime, sockfd, addrlen, connectresult);
}

void Log_SendOrRecv(int operation, char* commandname, int uid, int pid, char* buffer, int arg1, int arg2, int arg3, int ret) {
	char audit_type[32];
	if (operation == __NR_sendto) { strcpy(audit_type, "sendto"); }
	if (operation == __NR_recvfrom) { strcpy(audit_type, "recvfrom"); }
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char connectresult[10];
	int sockfd = arg1;
	int flags = arg2;
	int addrlen = arg3;
	if (ret == 0)
		strcpy(connectresult, "success");
	else
		strcpy(connectresult, "failed");

	time_t t = time(0);
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);

	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
	fprintf(logfile, "%s :%s(%d) %s %d  %d:%d %s\n", audit_type, username, uid, commandname, sockfd, flags, addrlen, connectresult);
	printf("%s:%s(%d) %s %d %d :%d %s\n", audit_type, username, uid, commandname, sockfd, flags, addrlen, connectresult);
}

void Log_Socket(char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret)
{
	char audit_type[32];
	strcpy(audit_type, "socket");
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	time_t t = time(0);
	if (logfile == NULL)	return;
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);
	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
	fprintf(logfile, "%s :%s(%d) %s(%d) %s \"%d\" %d %d\n", audit_type, username, uid, commandname, pid, logtime, arg1, arg2, arg3);
	printf(" %s :%s(%d) %s(%d) %s \"%d\" %d %d\n", audit_type, username, uid, commandname, pid, logtime, arg1, arg2, arg3);
}

void Log_module(int operation, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer) {
	char audit_type[32];
	if (operation == __NR_finit_module) { strcpy(audit_type, "initmodule"); }
	if (operation == __NR_delete_module) { strcpy(audit_type, "deletemodule"); }
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char result[10];

	if (ret == 0)
		strcpy(result, "success");
	else
		strcpy(result, "failed");

	time_t t = time(0);
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);
	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));

	fprintf(logfile, "%s :%s(%d) %s %d  %d %d %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime);
	printf("%s:%s(%d) %s %d %d %d %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime);

}

void Log_mountANDunmount(int op, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer, char* redundancy1, char* redundancy2) {
	char audit_type[32];
	if (operation == __NR_mount) { strcpy(audit_type, "mount"); }
	if (operation == __NR_umount2) { strcpy(audit_type, "deletemodule"); }
	char logtime[64];
	char username[32];
	struct passwd* pwinfo;
	char result[10];

	if (ret == 0)
		strcpy(result, "success");
	else
		strcpy(result, "failed");

	time_t t = time(0);
	pwinfo = getpwuid(uid);
	strcpy(username, pwinfo->pw_name);
	strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));

	fprintf(logfile, "%s :%s(%d) %s %d  %d %d %s %s %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime, redundancy1, redundancy2);
	printf("%s:%s(%d) %s %d %d %d %s %s %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime, redundancy1, redundancy2);


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
	iov.iov_base = (void*)nlh;
	iov.iov_len = nlh->nlmsg_len;
	msg.msg_name = (void*)&dest_addr;
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



void PreLog(struct nlmsghdr* nlh) {
	unsigned int uid, pid, ret, op;
	int arg1, arg2, arg3;
	char* buffer;
	char* commandname;
	char* redundancy1, * redundancy2;
	op = *((unsigned int*)NLMSG_DATA(nlh));
	uid = *(1 + (unsigned int*)NLMSG_DATA(nlh));
	pid = *(2 + (int*)NLMSG_DATA(nlh));
	arg1 = *(3 + (int*)NLMSG_DATA(nlh));
	arg2 = *(4 + (int*)NLMSG_DATA(nlh));
	arg3 = *(5 + (int*)NLMSG_DATA(nlh));
	ret = *(6 + (int*)NLMSG_DATA(nlh));
	commandname = (char*)(7 + (int*)NLMSG_DATA(nlh));
	buffer = (char*)(7 + 16 / 4 + (int*)NLMSG_DATA(nlh));
	switch (op) {
	case __NR_connect:
		Log_Connect(commandname, uid, pid, buffer, arg1, arg2, arg3, ret);
		break;
	case __NR_bind:
		Log_Bind(commandname, uid, pid, buffer, arg1, arg2, arg3, ret);
		break;
	case __NR_sendto:
	case __NR_recvfrom:
		Log_SendOrRecv(op, commandname, uid, pid, buffer, arg1, arg2, arg3, ret);
		break;
	case __NR_socket:
		Log_Socket(commandname, uid, pid, arg1, arg2, arg3, ret);
		break;
	case __NR_finit_module:
	case __NR_delete_module:
		Log_module(op, commandname, uid, pid, arg1, arg2, arg3, ret, buffer);
		break;
	case __NR_mount:
	case __NR_umount2:
		redundancy1 = (char*)(7 + 16 / 4 + strlen(buffer) / 4 + (int*)NLMSG_DATA(nlh));
		redundancy2 = (char*)(7 + 16 / 4 + strlen(buffer) / 4 + strlen(redundancy1) / 4 + (int*)NLMSG_DATA(nlh));
		Log_mountANDunmount(op, commandname, uid, pid, arg1, arg2, arg3, ret, buffer, redundancy1, redundancy2);
	case __NR_execve:
	case __NR_openat:
	case __NR_unlinkat:
	case __NR_write:
	case __NR_close:
	case __NR_read:
	case __NR_mknodat:
		//Log_file(op, commandname, uid, pid, buffer, arg1, arg2, arg3, ret);
		break;
	}
}




int main(int argc, char* argv[]) {
	char buff[110];
	//void killdeal_func();
	char logpath[32];
	if (argc == 1) strcpy(logpath, "./log");
	else if (argc == 2) strncpy(logpath, argv[1], 32);
	else {
		printf("commandline parameters error! please check and try it! \n");
		exit(1);
	}


	signal(SIGTERM, killdeal_func);
	sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
	nlh = (struct nlmsghdr*)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

	sendpid(getpid());

	/* open the log file at the begining of daemon, in case of this operation causes deadlock */
	logfile = fopen(logpath, "w+");
	if (logfile == NULL) {
		printf("Waring: can not create log file\n");
		exit(1);
	}
	//Loop to get message
	while (1) {	//Read message from kernel
		recvmsg(sock_fd, &msg, 0);
		PreLog(nlh);
	}
	close(sock_fd);
	free(nlh);
	fclose(logfile);
	return 0;
}
