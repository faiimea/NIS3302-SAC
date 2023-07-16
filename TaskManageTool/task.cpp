#include"task.h"
#include <QWidget>
int sock_fd;
struct msghdr msg;
struct nlmsghdr *nlh = NULL;
struct sockaddr_nl src_addr, dest_addr;
struct iovec iov;

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
        insert_log("ff","asd","asdasd","jljl","Success",6,7);
        insert_log("ff","ff","asjlk","jkaslkdljl","Success",0,9);
        sendpid(getpid());
        /* open the log file at the begining of daemon, in case of this operation causes deadlock */

        //Loop to get message
        while(1){	//Read message from kernel

            recvmsg(sock_fd, &msg, 0);
            //PreLog(nlh);

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
        sqlite3_free(zErrMsg);
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
        sqlite3_free(zErrMsg);
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


        sqlite3_free(zErrMsg);
       // sqlite3_close(db);
        return 0;
    } else if (n_rows == 0) return 0;
    else
    {
        fprintf(stdout, "Records searched successfully\n");
        printf("Found %d rows.\n", n_rows);
    return 1;
    }

}


void create_table( char username[])
{

    char *zErrMsg=0;
    char sql[300]="CREATE TABLE IF NOT EXISTS ";
    //printf(sql);s


    strcat(sql,username);
    char tail[300] = "(PID INT PRIMARY KEY    NOT NULL,LOGTIME TEXT NOT NULL,UID INT NOT NULL,USERNAME TEXT NOT NULL,"
                "FILEPATH        TEXT   NOT NULL,COMMANDNAME TEXT NOT NULL, RESULT TEXT NOT NULL);";
    strcat(sql,tail);

    printf(sql);

    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
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


void insert_log(char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid )
{
    char *zErrMsg = 0;
    int rc;
    mu.lock();

    // 开始事务
    rc=sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, &zErrMsg);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } ;
    // 执行插入操作
    char sql1[300] = "INSERT INTO ";
    char sql2[300]="(PID,LOGTIME,UID,USERNAME,FILEPATH,COMMANDNAME,RESULT) VALUES (?, ?, ?,?,?,?,?);";
    strcat(sql1,username);
    strcat(sql1,sql2);
    char *sql=sql1;
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
     if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } ;



    rc=sqlite3_bind_int(stmt, 1, pid);

    sqlite3_bind_text(stmt, 2, logtime,-1,SQLITE_TRANSIENT);
    sqlite3_bind_int(stmt, 3, uid);
    sqlite3_bind_text(stmt, 4, username,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 5, filepath,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 6, commandname,-1,SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 7,result,-1,SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Cannot execute statement: %s\n", sqlite3_errmsg(db));
        mu.unlock();
        return ;
    }


    // 提交事务
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    sqlite3_finalize(stmt);
    printf("insert successfully!\n");
    mu.unlock();
    return ;
}


void delete_log(char *username,int pid )
{
    int count=0;
    char *zErrMsg;
    char *sql=sqlite3_mprintf("DELETE FROM %s WHERE PID = %d;",username,pid);

    printf(sql);
    mu.lock();
    //sqlite3_open("database.db", &db);
    int rc = sqlite3_exec(db, sql,0, &count, &zErrMsg);

    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records deleted successfully\n");
        printf("Deleted %d rows.\n", count);
    }
    //sqlite3_close(db);
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


