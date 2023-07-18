#include "api.h"
#include <cstdio>

/*
rwAccount::rwAccount()
{
    mapUserAndHash.clear();
    readAccout();
}

rwAccount::~rwAccount()
{
    mapUserAndHash.clear();
}

rwAccount::readAccout()
{
    char s1[100], s2[100];
    FILE *fp = fopen("accout.txt", "r");
    if(fp == NULL) return;
    while(fscanf(fp, "%s %s", s1, s2) != EOF)
    {
        QString str1 = QString(s1);
        QByteArray str2 = QByteArray(s2);
        mapUserAndHash.insert(str1, str2);
    }
    fclose(fp);
}

rwAccount::writeAccout()
{
    FILE *fp = fopen("accout.txt", "w+");
    QMap<QString, QByteArray>::iterator it;
    for(it = mapUserAndHash.begin(); it != mapUserAndHash.end(); it++)
    {
        fputs(it.key().toStdString().c_str(), fp);
        fputs(" ", fp);
        fputs(it.value().data(), fp);
        fputs("\n", fp);
    }
    fclose(fp);
}
*/
