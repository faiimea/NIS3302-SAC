#ifndef TASK_H
#define TASK_H
#include <QObject>
#include <sqlite3.h>
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
#include <thread>
#include <QApplication>
#include "api.h"
#include <mutex>
#include "global.h"
#include "config.h"
#include "Func.h"
static sqlite3 *db;
static bool flag=true;
static std::mutex mu;
struct task{
    QString Username;
    QString Commondname;
    QString LogTime;
    QString File_path;
    QString Result;
    int pid;
    int uid;

};
void solve();
void _init_database ();

void create_users();

void remove_users();

void insert_users(char username[],char password[]);

void delete_users(char username[],char password[]);

int search_users(char username[],char password[]);

void create_table( char username[]);


int change_password(char username[],char origin_password[],char change_password[]);

void print_ip_address(unsigned int ip);

void sendpid(unsigned int pid);

void insert_log(char tablename[],char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid,char arg1[],char arg2[],char arg3[],char buffer[]);
void delete_log(char username[],int id );
void kill_func(int sig);
static int count_callback(void* data, int n_cols, char** values, char** columns);
static int callback_search(void *data, int argc, char **argv, char **azColName);

void Log_file(int operation, char* commandname, int uid, int pid, char* file_path, int arg1, int arg2, int arg3, int ret);
void Log_Connect(char* commandname, int uid, int pid, char* buffer, int port_int, int ip_int, int arg3, int ret) ;
void Log_Bind(char* commandname, int uid, int pid, char* buffer, int arg1, int arg2, int arg3, int ret);
void Log_SendOrRecv(int operation, char* commandname, int uid, int pid, char* buffer, int arg1, int arg2, int arg3, int ret);
void Log_Socket(char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret);
void Log_module(int operation, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer);
void Log_mountANDunmount(int op, char* commandname, int uid, int pid, int arg1, int arg2, int arg3, int ret, char* buffer, char* redundancy1, char* redundancy2);
void PreLog(struct nlmsghdr* nlh);
char* intToChar(int num);
#endif // TASK_H

