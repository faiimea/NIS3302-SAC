// 文件名：config.c
#include <config.h>

static int arr_curr_ind = 0;
static struct ConfigStruct ArrayConfig[CONFIGSTRUCTSIZE];

int LoadConfigFile(const char* pconfName)
{
    if(arr_curr_ind > 0){
        return 0;
    }
    FILE *fp;
    fp = fopen(pconfName, "r");
    if(fp == NULL)
        return false;

    // 每一行配置文件读取出来放到这里
    unsigned int linebuf_size = CONFIGCONTENTSIZE + 1 + CONFIGNAMESIZE + 1 + 1;
    char linebuf[linebuf_size];  // 128+1 结尾为结束符
    memset(linebuf, 0, sizeof(linebuf));
    while(!feof(fp))
    {
        if(fgets(linebuf, linebuf_size, fp) == NULL)
            continue;
        if(linebuf[0] == 0)
            continue;
        if(*linebuf == ';' || *linebuf == ' ' || *linebuf == '#' || *linebuf == '\t' || *linebuf == '\n')
            continue;

        // 去除字符串中的 \r \n 以及空格' '
    lblprocstring:
        if(strlen(linebuf) > 0)
        {
            if(linebuf[strlen(linebuf) - 1] == 10 || linebuf[strlen(linebuf) - 1] == 13 || linebuf[strlen(linebuf) - 1] == 32 )
            {
                linebuf[strlen(linebuf) - 1] = 0;
                goto lblprocstring;
            }
        }
        if(linebuf[0] == 0)
            continue;

        if(*linebuf == '[')  // [ 开头的注释，也保存，方便以后写文件时写回去
        {
            if(arr_curr_ind < CONFIGSTRUCTSIZE)
            {
                strcpy(ArrayConfig[arr_curr_ind].ItemName, linebuf);
                strcpy(ArrayConfig[arr_curr_ind].ItemContent, " ");
                // ArrayConfig[arr_curr_ind] = p_configitem;
                arr_curr_ind += 1;
            }
            else
            {
                // ERROR
            }
            continue;
        }

        // 到这里，都是合法的配置项
        char *ptmp = strchr(linebuf, '=');
        if(ptmp != NULL)
        {
            if(arr_curr_ind < CONFIGSTRUCTSIZE)
            {
                strncpy(ArrayConfig[arr_curr_ind].ItemName, linebuf, (int)(ptmp-linebuf));
                strcpy(ArrayConfig[arr_curr_ind].ItemContent, ptmp+1);
                Rtrim(ArrayConfig[arr_curr_ind].ItemName);
                Ltrim(ArrayConfig[arr_curr_ind].ItemName);
                Rtrim(ArrayConfig[arr_curr_ind].ItemContent);
                Ltrim(ArrayConfig[arr_curr_ind].ItemContent);
                arr_curr_ind += 1;
            }
            else
            {
                // ERROR
            }
        } // end if
    } // end while
    fclose(fp);
    return 1;
}

const char* GetConfigFromString(const char* p_itemname)
{
    int i = 0;
    for(i = 0; i < arr_curr_ind; i++)
    {
        if(strcmp(p_itemname, ArrayConfig[i].ItemName) == 0)
        {
            return ArrayConfig[i].ItemContent;
        }
    }
    return NULL;
}

int GetConfigIntDefault(const char* p_itemname, const int def)
{
    int i;
    for(i = 0; i < arr_curr_ind; i++)
    {
        if(strcmp(p_itemname, ArrayConfig[i].ItemName) == 0)
        {
            return atoi(ArrayConfig[i].ItemContent);
        }
    }
    return def;
}


int ModifyConfigItemContent(const char* p_itemName, const char* p_itemContent)
{
    if (!p_itemName || !p_itemContent)
        return 0;
    int ret_res = 0;
    int i = 0;
    for(i = 0; i < arr_curr_ind; i++)
    {
        if(strcmp(p_itemName, ArrayConfig[i].ItemName) == 0)
        {
            strcpy(ArrayConfig[i].ItemContent, p_itemContent);
            ret_res = 1;
        }
    }
    return ret_res;
}

int AddConfigItem(const char*p_itemName, const char* p_itemContent)
{
    if (!p_itemName || !p_itemContent)
        return 0;
    int i;
    int ret_res = 0;
    for(i=0; i<arr_curr_ind; ++i)
    {
        ret_res = ModifyConfigItemContent(p_itemName, p_itemContent);
        if(ret_res == 1)
            return ret_res;
    }
    ret_res = 0;
    if(arr_curr_ind < CONFIGSTRUCTSIZE)
    {
        strcpy(ArrayConfig[arr_curr_ind].ItemName, p_itemName);
        strcpy(ArrayConfig[arr_curr_ind].ItemContent, p_itemContent);
        arr_curr_ind += 1;
        ret_res = 1;
    }
    else
    {
        ret_res = 0;
    }
    return ret_res;
}

int WriteNewConfigFile(const char*pconfName)
{
    if(!pconfName)
        return 0;
    int new_fd;
    if(-1 == (new_fd = open(pconfName,O_RDWR|O_CREAT|O_TRUNC,0664)))
    {
        return 0;
    }
    int i = 0;
    char line_buf[32+128+1] = "\0";
    char equal_char = '=';
    FILE* script_file = fopen("../kernel_module/script.sh", "w");
    if (script_file == NULL) {
        printf("无法创建脚本文件\n");
        close(new_fd);
        return 0;
    }
    fprintf(script_file, "#!/bin/bash\n");
    fprintf(script_file, "sudo insmod AuditModule.ko ");
    for(i=0; i < arr_curr_ind; i++)
    {
        if(*(ArrayConfig[i].ItemName) == '[')
            equal_char = ' ';
        else
            equal_char = '=';
        sprintf(line_buf, "%s%c%s\n", ArrayConfig[i].ItemName, equal_char, ArrayConfig[i].ItemContent);
        write(new_fd, line_buf, strlen(line_buf));
        int len=strlen(line_buf);
        if (len > 0 && line_buf[len - 1] == '\n') {
        line_buf[len - 1] = ' ';
        line_buf[len] = '\0';
    }
        fprintf(script_file, "%s", line_buf);
        memset(line_buf, 0, sizeof(line_buf));
    }
    fprintf(script_file, "\n");
    fclose(script_file);
    close(new_fd);
    return 1;
}
