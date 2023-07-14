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

void insert_log(char username[],char logtime[],char filepath[],char commandname[],char result[],int pid,int uid );

void delete_log(char username[],int pid );
void kill_func();





#endif // TASK_H

