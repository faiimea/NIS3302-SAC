// 文件名：Func.h
#ifndef __Func_h__
#define __Func_h__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"

void Rtrim(char* sstring);
void Ltrim(char* sstring);

//比较str1和str2两个字符串内容是否一样，忽略大小写的比较
bool StrcaseCmp(const char* str1, const char* str2);

//把str全都变成大写
void StringCase(char* str);

#endif // !__Func_h__
