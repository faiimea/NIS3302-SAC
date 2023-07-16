// 文件名：Func.c
#include "Func.h"

//截取字符串尾部空格
void Rtrim(char *sstring)
{
	size_t len = 0;
	if (sstring == NULL)
		return;
	len = strlen(sstring);
	while (len > 0 && sstring[len - 1] == ' ')//位置换一下
		sstring[--len] = 0;
}

//截取字符串首部空格
void Ltrim(char *sstring)
{
	size_t len = 0;
	if (sstring == NULL)
		return;
	char *p_tmp = sstring;  //此时p_tmp指向了sstring
	if ((*p_tmp) != ' ')
		return;  //不是以空格开头

	//找到第一个不为空格的位置
	while ((*p_tmp) != '\0')
	{
		if ((*p_tmp) == ' ')
			p_tmp++;
		else
			break;
	}
	if ((*p_tmp) == '\0') //全是空格
	{
		*sstring = '\0';
		return;
	}
	char *p_tmp2 = sstring;  //此时 p_tmp2 指向了 sstring
	while ((*p_tmp2) != '\0')
	{
		(*p_tmp2) = (*p_tmp);
		p_tmp++;
		p_tmp2++;
	}
	(*p_tmp2) = '\0';  //空格处理完毕，记得在末尾添加一个'\0' 表示字符串结尾
	return;
}

//比较str1和str2两个字符串内容是否一样，忽略大小写的比较
bool StrcaseCmp(const char* str1, const char* str2)
{
	if (str1 == NULL || str2 == NULL)
		return false;
	size_t strLen1 = strlen(str1);
	size_t strLen2 = strlen(str2);
	if (strLen1 != strLen2)
		return false;
	// char *str = new char[strLen2]; 
    char *str = (char*)malloc(strLen2);
	strcpy(str, str2); //把str2的内容拷贝到str中
	StringCase(str);  //把str中的所有内容转成大写字母
	while ((*str) != '\0')
	{
		if ((*str1) > 90)
		{
			if (((*str1) - 32) == (*str))
			{
				++str1;
				++str;
			}
			else {
				return false;
			}
		}
		else {
			if ((*str1) == (*str))
			{
				++str1;
				++str;
			}
			else {
				return false;
			}
		}
	}
    free(str);
	return true;
}

//把str中所有的小写字母变成大写
void StringCase(char* str)
{
	if (str == NULL)
		return ;
	if (strlen(str) <= 0)
		return ;
	int strLen = strlen(str);
	char *ptmp = str;
	while ((*ptmp) != '\0')
	{
		if ((97 <= (*ptmp)) && ((*ptmp) <= 122))
		{
			(*ptmp) -= 32;
		}
		++ptmp;
	}
	return;
}
