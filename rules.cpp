#include "rules.h"


bool TextInputRule::IsEnglish(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z'))
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}


bool TextInputRule::IsNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if(*s>='0' && *s<='9')
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}

bool TextInputRule::IsChinese(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>= 0x4E00) && (*s<= 0x9FA5))
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}


bool TextInputRule::IsEnglishOrNumber(QString &qstrSrc)
{
    QByteArray ba = qstrSrc.toLatin1();
    const char *s = ba.data();
    bool bret = true;
    while(*s)
    {
        if((*s>='A' && *s<='Z') || (*s>='a' && *s<='z') || (*s>='0' && *s<='9'))
        {

        }
        else
        {
            bret = false;
            break;
        }
        s++;
    }
    return bret;
}

bool TextInputRule::IsChineseOrNumber(QString &qstrSrc)
{
    int nCount = qstrSrc.count();
    bool bret = true;
    for(int i=0; i<nCount; ++i)
    {
        QChar cha = qstrSrc.at(i);
        ushort uni = cha.unicode();
        if((uni >= 0x4E00 && uni <= 0x9FA5)  || (uni >= '0' && uni <= '9'))
        {

        }
        else
        {
            bret = false;
            break;
        }
    }
    return bret;
}




