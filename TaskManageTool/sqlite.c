#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <linux/socket.h>
#include <fcntl.h>
#include <asm/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <pwd.h>
#include <sqlite3.h>
char username[100];
sqlite3 *db;
static int callback_search(void *data, int argc, char **argv, char **azColName) ;
static int count_callback(void* data, int n_cols, char** values, char** columns);
void _init_database ()
{
    int rc;
    rc = sqlite3_open("database.db", &db);

    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   
        return;
    } else {
        fprintf(stdout, "Opened database successfully\n");
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
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
 
	}
	return;
}
void create_users()
{
    remove_users();
	char *zErrMsg=0;	
    
	char *sql = "CREATE TABLE USERS("
                "USERNAME TEXT PRIMARY KEY NOT NULL,"
				"PASSWORD TEXT NOT NULL);";
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
	
	
	printf(sql);
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
    rc = sqlite3_exec(db,sql,callback_search,0,&zErrMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	
		return 0;
	} else if (n_rows==0){

    }        else
    {
        fprintf(stdout, "Records searched successfully\n");
        printf("Found %d rows.\n", n_rows);
	    return 1;
    }
	
}
void delete_table(char username[])
{
     char *zErrMsg=0;	
    char sql[200]="DROP TABLE IF EXISTS ";
    strcat(sql,username);
    
    int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
	
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table deleted successfully\n");
 
	}
	return;
}
void create_table(char username[])
{
    
    char *zErrMsg=0;	
    char sql[200]="CREATE TABLE IF NOT EXISTS ";
    strcat(sql,username);
    char tail[300] = "(PID INT PRIMARY KEY    NOT NULL,LOGTIME TEXT NOT NULL,UID INT NOT NULL,USERNAME TEXT NOT NULL,"
                "FILEPATH		 TEXT   NOT NULL,COMMANDNAME TEXT NOT NULL, RESULT TEXT NOT NULL);";
    strcat(sql,tail);
	
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


void insert_log(char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid )
{
    char *zErrMsg = 0;
    int rc;
   
    // 开始事务
    rc=sqlite3_exec(db, "BEGIN TRANSACTION", 0, 0, zErrMsg);
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
  
        return 1;
    }
    

    // 提交事务
    sqlite3_exec(db, "COMMIT TRANSACTION", 0, 0, 0);
    sqlite3_finalize(stmt);
    printf("insert successfully!\n");
    return 0;
}

void delete_log(char username[],int pid )
{
    char sql[200]="DELETE FROM ";
    char mid[100]= " WHERE PID =";

	char tail[25]=";";
	char *zErrMsg=0;	
    int count = 0;
	strcat(sql,username);
	strcat(sql,mid);
    char pids[100];
    sprintf(pids,"%d",pid);

	strcat(sql,pids);
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
    
}

void search_log(char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid)
{
    char sql[400]="SELECT * FROM ";
    char where[20]=" WHERE ";

    char username_str[30]="USERNAME LIKE '%%";
    char logtime_str[30]="LOGTIME LIKE '%%";
    char filepath_str[30]="FILEPATH LIKE '%%";
    char commandname_str[30]="COMMANDNAME LIKE '%%";
    char result_str[30]="RESULT = '";
    char pid_str[20]="PID = ";
    char uid_str[20]="UID = ";
    char pids[20];
    char uids[20];
	char tail[10]="%%'";
    char tail1
    char tail2[5]=";";
	char *zErrMsg=0;	
    int count = 0;

    
    printf("%s\n",logtime);
    
    strcat(sql,username);
    strcat(sql,where);
    
	strcat(sql,username_str);
    strcat(sql,username);
    strcat(sql,tail);

    strcat(sql,logtime_str);
    strcat(sql,logtime);
    strcat(sql,tail);

    strcat(sql,filepath_str);
    strcat(sql,filepath);
    strcat(sql,tail);

    strcat(sql,commandname_str);
    strcat(sql,commandname);
    strcat(sql,tail);

    strcat(sql,result_str);
    strcat(sql,result);
    strcat(sql,tail);

    sprintf(pids,"%d",pid);
    strcat(sql,pid_str);
    strcat(sql,pids);

    sprintf(uids,"%d",uid);
    strcat(sql,uid_str);
    strcat(sql,uids);
    strcat(sql,tail2);

	printf(sql);
    int rc = sqlite3_exec(db, sql, callback_search, &count, &zErrMsg);

   if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	
		return 0;
	} else if (count==0){

    }        else
    {
        fprintf(stdout, "Records searched successfully\n");
        printf("Found %d rows.\n", count);
	   
    }
}

void update_log()
{
    
    static int initial_changes=0;
    /* 记录下第一次查询后的总更改数 */
    

    /* 执行 SQL 语句，例如插入、更新或删除操作 */

    /* 计算自上次查询以来所做的更改数，从而获得新增的内容 */
    int new_changes = sqlite3_total_changes(db) - initial_changes;
    if (new_changes > 0) {
        char *sql = sqlite3_mprintf("SELECT * FROM %Q ORDER BY last_updated DESC LIMIT %d;", tableName,new_changes);
        rc = sqlite3_prepare_v2(db, sql, , &stmt, 0);
    }
    /* 关闭数据库连接 */
    initial_changes = new_changes;
    return 0;

}

int main()
{
    _init_database();
    create_users();
    insert_users("hello","world");
    insert_users("alex","2");
    insert_users("alex","111");
    insert_users("please","111");
    change_password("alex","111","222");
    change_password("please","222","222");
    create_table("alex");
   
    search_users("alex","111");
    delete_users("hello","world");
    delete_users("alex","111");
    change_password("alex","111",'222');
    insert_log("alex","11:11:11","auditpath/1/2","read/write","successful",11,22);
    insert_log("zts","123","dff","read","unsuccessful",12,33);
    insert_log("zts","123","dff","read","unsuccessful",13,33);
    int rc=sqlite3_exec(db,"SELECT * FROM alex;",callback_search,0,0);
    delete_log("alex",11);
    rc=sqlite3_exec(db,"SELECT * FROM alex;",callback_search,0,0);
    char *username="alex";
    char *logtime="";
    char *filepath="";
    char *commandname="";
    char *result="successful";
    int pid=12;
    int uid =33;
    search_log(username,logtime,filepath,commandname,result,pid,uid);
}

static int callback_search(void *data, int argc, char **argv, char **azColName) {
    int i;
     // Process the retrieved data as needed
    for (i = 0; i < argc; i++) {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    return 0;
}
static int count_callback(void* data, int n_cols, char** values, char** columns) {
    int* count = (int*)data;
    (*count)++;
    return 0;
}
