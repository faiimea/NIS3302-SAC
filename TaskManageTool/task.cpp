#include"task.h"
#include <QWidget>
int sock_fd;
struct msghdr msg;
struct nlmsghdr *nlh = NULL;
struct sockaddr_nl src_addr, dest_addr;
struct iovec iov;
#define TM_FMT "%Y-%m-%d %H:%M:%S"
#define NETLINK_TEST 29
#define MAX_PAYLOAD 1024  /* maximum payload size*/
void solve(){
    char buff[110];
        //void killdeal_func();
        char logpath[32];
        signal(SIGTERM,kill_func);
        sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_TEST);
        nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
        memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));

        sendpid(getpid());
        /* open the log file at the begining of daemon, in case of this operation causes deadlock */

        //Loop to get message
        while(1){	//Read message from kernel
		if(flag){
            recvmsg(sock_fd, &msg, 0);
            PreLog(nlh);
            }

        }

        ::close(sock_fd);
        free(nlh);


}

void _init_database ()
{
    int rc;
    rc = sqlite3_open("database.db", &db);
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    return;
}



void create_users()
{
    char *zErrMsg=0;

    char *sql = "CREATE TABLE IF NOT EXISTS USERS("
                "USERNAME TEXT NOT NULL,"
                "PASSWORD TEXT NOT NULL);";
    printf(sql);

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);

    } else {
        fprintf(stdout, "Table created successfully\n");
    }
    return;
}


void remove_users()
{
    char *zErrMsg=0;

    char *sql = "DROP TABLE USERS";
    printf("%s\n",sql);
     int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);

    } else {
        fprintf(stdout, "Table created successfully\n");

    }
    return;
}

void insert_users(char username[],char password[])
{
    char tmp[64];
    char sql[200]="INSERT INTO USERS (USERNAME,PASSWORD) VALUES('";
    char gap[5]="','";
    char tail[20]="');";
    char *zErrMsg=0;

    strcat(sql,username);
    strcat(sql,gap);
    strcat(sql,password);
    strcat(sql,tail);


    //printf(sql);
    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        //sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");

    }
    return;

}


void delete_users(char username[],char password[])
{
    mu.lock();
    char sql[200]="DELETE FROM USERS WHERE USERNAME='";
    char gap[20]="' AND PASSWORD='";
    char tail[25]="';";
    char *zErrMsg=0;
    int count = -1;
    strcat(sql,username);
    strcat(sql,gap);
    strcat(sql,password);
    strcat(sql,tail);


    printf(sql);
    int rc = sqlite3_exec(db, sql, count_callback, &count, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        //sqlite3_free(zErrMsg);
    } else {
           fprintf(stdout, "Records deleted successfully\n");
        printf("Deleted %d rows.\n", count);
    }
    mu.unlock();

}

int search_users(char username[],char password[])
{

    char sql[200]="SELECT * FROM USERS WHERE USERNAME='";
    char gap[20]="' AND PASSWORD='";
    char tail[25]="';";
    strcat(sql,username);
    strcat(sql,gap);
    strcat(sql,password);
    strcat(sql,tail);
    printf(sql);

    char* zErrMsg = 0;

    int n_rows=0;
    int rc = sqlite3_exec(db, sql, count_callback, &n_rows,  &zErrMsg);
    if (rc != SQLITE_OK) {
        //fprintf(stderr, "SQL error: %s\n", zErrMsg);


       // sqlite3_free(zErrMsg);
        //sqlite3_close(db);
        return 0;
    } else if (n_rows == 0) return 0;
    else
    {
        create_table(username);
        fprintf(stdout, "Records searched successfully\n");
        printf("Found %d rows.\n", n_rows);
    return 1;
    }

}


void create_table(char username[])
{

    char *zErrMsg=0;
    char sql[500]="CREATE TABLE IF NOT EXISTS ";
    strcat(sql,username);
    char tail[500] = "(ID INTEGER PRIMARY KEY AUTOINCREMENT, PID INT   NOT NULL,LOGTIME TEXT NOT NULL,UID INT NOT NULL,USERNAME TEXT NOT NULL,"
                "FILEPATH        TEXT   NOT NULL,COMMANDNAME TEXT NOT NULL, RESULT TEXT NOT NULL, ARG1 TEXT  NULL, ARG2 TEXT NULL, ARG3 TEXT NULL,BUFFER TEXT NULL);";
    strcat(sql,tail);

    printf(sql);
    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));

    } else {
        fprintf(stdout, "Table created successfully\n");
    }
    return;
}




int change_password(char username[],char origin_password[],char change_password[])
{
    if (search_users(username,origin_password)==1){
        delete_users(username,origin_password);
        insert_users(username,change_password);
        fprintf(stdout,"successfully update!\n");
        return 1;
    }
    else
    {
        printf("Unsuccessfully update!\n");
        return 0;
    }
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
void insert_log(char tablename[],char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid,char arg1[],char arg2[],char arg3[],char buffer[])
{
    char *zErrMsg = 0;
    int rc;
   mu.lock();

    // 开始事务
   /* rc=sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } ;*/
    // 执行插入操作
    char sql1[300] = "INSERT INTO ";
    char sql2[300]="(PID,LOGTIME,UID,USERNAME,FILEPATH,COMMANDNAME,RESULT,ARG1,ARG2,ARG3,BUFFER) VALUES (?, ?, ?,?,?,?,?,?,?,?,?);";
    strcat(sql1,tablename);
    strcat(sql1,sql2);
    char *sql=sql1;
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }



    rc=sqlite3_bind_int(stmt, 1, pid);

    sqlite3_bind_text(stmt, 2, logtime,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, uid);
    sqlite3_bind_text(stmt, 4, username,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, filepath,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, commandname,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7,result,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 8,arg1,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 9,arg2,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 10,arg3,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 11,buffer,-1,SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        mu.unlock();
        return ;
    }


    // 提交事务
    //sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    sqlite3_finalize(stmt);
    printf("insert successfully!\n");
    mu.unlock();
    return ;
}



void delete_log(char *username,int id )
{
    int count;
    char *zErrMsg;
    char *sql=sqlite3_mprintf("DELETE FROM %s WHERE ID = %d;",username,id);

    printf(sql);
    mu.lock();
    int rc = sqlite3_exec(db, sql, count_callback, &count, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        sqlite3_exec(db, "COMMIT", nullptr, nullptr, &zErrMsg);
        fprintf(stdout, "Records deleted successfully\n");
        printf("Deleted %d rows.\n", count);
    }
    mu.unlock();


}
static int callback_search(void *data, int argc, char **argv, char **azColName) {

   int i;
   for(i = 0; i<argc; i++) {
       printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;

}
static int count_callback(void* data, int n_cols, char** values, char** columns) {
   int* count = (int*)data;
   (*count)++;
   return 0;
}

void kill_func(int sig)
{
    printf("The process is killed! \n");
    close(sock_fd);
    if (nlh != NULL)
        free(nlh);
    exit(0);
}

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

    pwinfo = getpwuid(uid);
    strcpy(username, pwinfo->pw_name);

    strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
   // fprintf(logfile, "%s:%s(%d) %s(%d) %s \"%s\" %s %s\n", audit_type, username, uid, commandname, pid, logtime, file_path, opentype, openresult);
    if (operation == __NR_openat) { strcpy(audit_type, "openat"); }
    if (operation == __NR_unlinkat) { strcpy(audit_type, "unlinkat"); }
    if (operation == __NR_execve) { strcpy(audit_type, "execve"); }
    if (operation == __NR_read) { strcpy(audit_type, "read"); }
    if (operation == __NR_close) { strcpy(audit_type, "close"); }
    if (operation == __NR_write) { strcpy(audit_type, "write"); }
    if (operation == __NR_mknodat) { strcpy(audit_type, "mknodat"); }
    if (operation == __NR_openat) { strcpy(audit_type, "openat"); }
    if (operation == __NR_finit_module) { strcpy(audit_type, "initmodule"); }
    if (operation == __NR_delete_module) { strcpy(audit_type, "deletemodule"); }
    if (operation == __NR_mount) { strcpy(audit_type, "mount"); }
    if (operation == __NR_umount2) { strcpy(audit_type, "umount"); }
    strcat(audit_type,": ");
    strcat(audit_type,opentype);
    if(strcmp(username,"root")!=0)
    insert_log("root",username,logtime,file_path,audit_type,openresult,pid,uid,NULL,NULL,NULL,NULL);
    insert_log(username,username,logtime,file_path,audit_type,openresult,pid,uid,NULL,NULL,NULL,NULL);



    printf("%s:%s(%d) %s(%d) %s \"%s\" %s %s\n", audit_type, username, uid, commandname, pid, logtime, file_path, opentype, openresult);
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
    std::string str = "port:"+std::to_string(port);
        char* charArray = new char[str.length() + 1];
        strcpy(charArray, str.c_str());
        if(strcmp(username,"root")!=0)
        insert_log("root",username,logtime,"",audit_type,connectresult,pid,uid,ip_str,charArray,NULL,NULL);
        insert_log(username,username,logtime,"",audit_type,connectresult,pid,uid,ip_str,charArray,NULL,NULL);


    delete charArray;

    //fprintf(logfile, "%s :%s(%d) %s(%d) %s  %s:%d %s\n", audit_type, username, uid, commandname, pid, logtime, ip_str, port, connectresult);
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
    //fprintf(logfile, "%s :%s(%d) %s(%d) %s  %d:%d %s\n", audit_type, username, uid, commandname, pid, logtime, sockfd, addrlen, connectresult);
    printf("%s:%s(%d) %s(%d) %s sockfd= %d:addrlen= %d %s\n", audit_type, username, uid, commandname, pid, logtime, sockfd, addrlen, connectresult);
    char*sock =intToChar(sockfd),*ad=intToChar(addrlen);
    if(strcmp(username,"root")!=0)
    insert_log("root",username,logtime,"",audit_type,connectresult,pid,uid,sock,ad,NULL,NULL);
    insert_log(username,username,logtime,"",audit_type,connectresult,pid,uid,sock,ad,NULL,NULL);
    delete []sock,ad;
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
    //fprintf(logfile, "%s :%s(%d) %s %d  %d:%d %s\n", audit_type, username, uid, commandname, sockfd, flags, addrlen, connectresult);
    printf("%s:%s(%d) %s %d %d :%d %s\n", audit_type, username, uid, commandname, sockfd, flags, addrlen, connectresult);
    char*sock =intToChar(sockfd),*ad=intToChar(addrlen),*fl=intToChar(flags);
    if(strcmp(username,"root")!=0)
    insert_log("root",username,logtime,"",audit_type,connectresult,pid,uid,sock,ad,fl,NULL);
    insert_log(username,username,logtime,"",audit_type,connectresult,pid,uid,sock,ad,fl,NULL);

    delete []sock,ad,fl;
}
void Log_Socket(char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret)
{
    char audit_type[32];
    strcpy(audit_type, "socket");
    char logtime[64];
    char username[32];
    struct passwd* pwinfo;
    time_t t = time(0);
    //if (logfile == NULL)	return;
    pwinfo = getpwuid(uid);
    strcpy(username, pwinfo->pw_name);
    strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));
    //fprintf(logfile, "%s :%s(%d) %s(%d) %s \"%d\" %d %d\n", audit_type, username, uid, commandname, pid, logtime, arg1, arg2, arg3);
    printf(" %s :%s(%d) %s(%d) %s \"%d\" %d %d\n", audit_type, username, uid, commandname, pid, logtime, arg1, arg2, arg3);
    char*a1 =intToChar(arg1),*a2=intToChar(arg2),*a3=intToChar(arg3);
    if(strcmp(username,"root")!=0)
    insert_log("root",username,logtime,"",audit_type,"",pid,uid,a1,a2,a3,NULL);
    insert_log(username,username,logtime,"",audit_type,"",pid,uid,a1,a2,a3,NULL);
    delete []a1,a2,a3;
}

void Log_module(int operation, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer) {
    char audit_type[32];
    if (operation == __NR_finit_module) { strcpy(audit_type, "initmodule"); }
    if (operation == __NR_delete_module) { strcpy(audit_type, "deletemodule"); }
    char logtime[64];
    char username[32];
    struct passwd* pwinfo;
    char result[10];

    if (ret == -1)
            strcpy(result, "failed");
        else
            strcpy(result, "success");

    time_t t = time(0);
    pwinfo = getpwuid(uid);
    strcpy(username, pwinfo->pw_name);
    strftime(logtime, sizeof(logtime), TM_FMT, localtime(&t));

    //fprintf(logfile, "%s :%s(%d) %s %d  %d %d %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime);
    printf("%s:%s(%d) %s %d %d %d %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime);
    if(strcmp(username,"root")!=0)
    insert_log("root",username,logtime,"",audit_type,result,pid,uid,NULL,NULL,NULL,buffer);
    insert_log(username,username,logtime,"",audit_type,result,pid,uid,NULL,NULL,NULL,buffer);

}

void Log_mountANDunmount(int op, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer, char* redundancy1, char* redundancy2) {
    char audit_type[32];
    if (op == __NR_mount) { strcpy(audit_type, "mount"); }
    if (op == __NR_umount2) { strcpy(audit_type, "deletemodule"); }
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
    if (op == __NR_mount) {
            printf("%s:%s(%d) %s %d %d %d %s %s %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime, redundancy1, redundancy2);
            if(strcmp(username,"root")!=0)
            insert_log("root",username,logtime,"",audit_type,"",pid,uid,redundancy1,redundancy2,NULL,buffer);
            insert_log(username,username,logtime,"",audit_type,"",pid,uid,redundancy1,redundancy2,NULL,buffer);
            }
        if (op == __NR_umount2) {
            //fprintf(logfile, "%s :%s(%d) %s %d  %d %d %s %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime, redundancy1);
            printf("%s:%s(%d) %s %d %d %d %s %s %s %s\n", audit_type, username, uid, commandname, 0, 0, 0, result, buffer, logtime, redundancy1);
            if(strcmp(username,"root")!=0)
            insert_log("root",username,logtime,"",audit_type,"",pid,uid,redundancy1,NULL,NULL,buffer);
            insert_log(username,username,logtime,"",audit_type,"",pid,uid,redundancy1,NULL,NULL,buffer);

            }

}

void PreLog(struct nlmsghdr* nlh) {
    const char* config_file = "./config.conf";
    LoadConfigFile(config_file);
    const int Audit_Connect = GetConfigIntDefault("connect",1);
    const int Audit_Bind = GetConfigIntDefault("bind", 1);
    const int Audit_Sendto = GetConfigIntDefault("sendto", 1);
    const int Audit_Recvfrom = GetConfigIntDefault("recvfrom", 1);
    const int Audit_Socket = GetConfigIntDefault("socket", 1);
    const int Audit_Finit_Module = GetConfigIntDefault("finit_module", 1);
    const int Audit_Delete_Module = GetConfigIntDefault("delete_module", 1);
    const int Audit_Mount = GetConfigIntDefault("mount", 1);
    const int Audit_Umount2 = GetConfigIntDefault("umount2", 1);
    const int Audit_Execve = GetConfigIntDefault("execve", 1);
    const int Audit_Openat = GetConfigIntDefault("openat", 1);
    const int Audit_Unlinkat = GetConfigIntDefault("unlinkat", 1);
    const int Audit_Write = GetConfigIntDefault("write", 1);
    const int Audit_Close = GetConfigIntDefault("close", 1);
    const int Audit_Read = GetConfigIntDefault("read", 1);
    const int Audit_Mknodat = GetConfigIntDefault("mknodat", 1);

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
        if(Audit_Connect==1)	printf("INFO:AuditConnect=1\n");
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
       Log_file(op, commandname, uid, pid, buffer, arg1, arg2, arg3, ret);
        break;
    }
}
int Change_Conf(){
    const char* config_file = "./config.conf";
    LoadConfigFile(config_file);

    char input[100];
    char itemName[50];
    char itemValue[50];

    printf("\n===== 配置项列表 =====\n");
    printf("1. audit_bind\n");
    printf("2. audit_sendto\n");
    printf("3. audit_recvfrom\n");
    printf("4. audit_socket\n");
    printf("5. audit_finit_module\n");
    printf("6. audit_delete_module\n");
    printf("7. audit_mount\n");
    printf("8. audit_umount2\n");
    printf("9. audit_execve\n");
    printf("10. audit_openat\n");
    printf("11. audit_unlinkat\n");
    printf("12. audit_write\n");
    printf("13. audit_close\n");
    printf("14. audit_read\n");
    printf("15. audit_mknodat\n");
    printf("q. 保存并退出\n");

    while(1){
        printf("请输入要修改的配置项的编号 (输入q保存并退出): ");
        scanf("%s", input);

        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0){
            // 输入q时保存配置项并退出
            WriteNewConfigFile(config_file);
            printf("配置项已保存并退出。\n");
            break;
        }

        int itemIndex = atoi(input);

        switch(itemIndex){
            case 1:
                strcpy(itemName, "audit_bind");
                break;
            case 2:
                strcpy(itemName, "audit_sendto");
                break;
            case 3:
                strcpy(itemName, "audit_recvfrom");
                break;
            case 4:
                strcpy(itemName, "audit_socket");
                break;
            case 5:
                strcpy(itemName, "audit_finit_module");
                break;
            case 6:
                strcpy(itemName, "audit_delete_module");
                break;
            case 7:
                strcpy(itemName, "audit_mount");
                break;
            case 8:
                strcpy(itemName, "audit_umount2");
                break;
            case 9:
                strcpy(itemName, "audit_execve");
                break;
            case 10:
                strcpy(itemName, "audit_openat");
                break;
            case 11:
                strcpy(itemName, "audit_unlinkat");
                break;
            case 12:
                strcpy(itemName, "audit_write");
                break;
            case 13:
                strcpy(itemName, "audit_close");
                break;
            case 14:
                strcpy(itemName, "audit_read");
                break;
            case 15:
                strcpy(itemName, "audit_mknodat");
                break;
            default:
                printf("无效的选择，请重新输入。\n");
                continue;
        }

        printf("请选择要设置的值 (0 或 1): ");
        scanf("%s", input);

        if (strcmp(input, "0") == 0){
            strcpy(itemValue, "0");
        }
        else if (strcmp(input, "1") == 0){
            strcpy(itemValue, "1");
        }
        else{
            printf("无效的选择，请重新输入。\n");
            continue;
        }

        // 修改配置项的值
        if (ModifyConfigItemContent(itemName, itemValue) == -1){
            // 如果修改失败，则尝试添加新的配置项
            if (AddConfigItem(itemName, itemValue) == -1){
                printf("修改或添加配置项失败，请稍后重试。\n");
                continue;
            }
        }

        printf("配置项已修改。\n");
    }

    return 0;
}

char* intToChar(int num)
{
    // 将 int 转换为字符串
    std::string str = std::to_string(num);
    // 将字符串复制到 char* 中
    char* charArray = new char[str.length() + 1];
    strcpy(charArray, str.c_str());
    return charArray;
}
