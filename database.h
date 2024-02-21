#ifndef DATABASE_H
#define DATABASE_H

//用于与数据库服务器交互的一切操作

#include<iostream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QString>
#include <QMessageBox>
#include <QDateTime>
#include "config.h"
using namespace std;



class CommunicateWithServer{


public:
    CommunicateWithServer(){}
    void downloadTable();

    void downloadUserInfo();

    void uploadFileToNextcloud(const QString &localFilePath, const QString &nextcloudUrl, const QString &username, const QString &password);
};

// 用户信息
class UserInfo
{
public:
    void addData(QString Qname, QString Qpassword, QString Qpermission);
    void deleteData(QString name);
    // symbol: 0->name, 1->password, 2->permission
    void modifyData(QString name, QString info, int symbol);

    bool findData(QString Qname);
    bool findData(QString name,QString Qpassword, QString &permission);

    // 获取第i个数据
    QStringList getIndexInfo(int index);
    QList<QStringList> getAllNamePerimission();
    // 超级管理员的权限，能看到所有用户的所有信息，包括密码
    QList<QStringList> getAllInfo();
    QStringList getSelfInfo(QString name);

    void Register();

private:
    string username_;
    string password_;
    string permission_; // 用户权限
};




#endif // DATABASE_H
