// 文件名：config.h
#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Func.h"
#include <fcntl.h>
#include <unistd.h>

// item数组的大小，也是配置文件中有效的item项目的最大值
#define CONFIGSTRUCTSIZE 80
#define CONFIGNAMESIZE 32
#define CONFIGCONTENTSIZE 512

struct ConfigStruct{
    char ItemName[CONFIGNAMESIZE];
    char ItemContent[CONFIGCONTENTSIZE];
};

// 加载配置文件
int LoadConfigFile(const char* pconfName);
// 通过item名字获取item的内容
const char* GetConfigFromString(const char* p_itemname);
// 通过item名字获取item内容，整型，获取不到返回默认值
int GetConfigIntDefault(const char* p_itemname, const int def);

// // 释放内存
// void FreeConfigItem();

// 写配置文件
// 通过item名字修改item内容
int ModifyConfigItemContent(const char* p_itemName, const char* p_itemContent);
// 通过item名字和内容 添加新的item项目
int AddConfigItem(const char*p_itemName, const char* p_itemContent);
// 把加载进来的、或者有修改过了、增加过的 item数组写到指定的文件
int WriteNewConfigFile(const char*pconfName);

#endif // !__CONFIG_H__
