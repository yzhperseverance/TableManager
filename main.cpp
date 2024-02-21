#include "widget.h"
#include "appinit.h"
#include "login.h"
#include "database.h"
#include "register.h"
#include "config.h"
#include <QApplication>
#include <QFile>
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //加载样式表
    QFile file(":/css/index.css");
    if (file.open(QFile::ReadOnly)) {
        QString qss = QLatin1String(file.readAll());
        qApp->setStyleSheet(qss);
        file.close();
    }

    //全局字体
    QFont font("Arial", 10);
    a.setFont(font);

    config::readConfigFile();


//    //屏幕拖动
    AppInit::Instance()->start();
    Widget w;
    login l(&w);
    l.show();
    //w.openWindow("yzh", "超级管理员");
//         return 0;
    return a.exec();
}



/*
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSslConfiguration>
#include <QFile>
#include "database.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("http://38.47.122.83/remote.php/dav/files/yzh/test.json"));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg("yzh").arg("yzh86015177").toLatin1().toBase64()));

    QFile file("E:\\software\\peach\\info");

    QNetworkReply* reply = manager.put(request, &file);


    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Upload successful!";
        } else {
            qDebug() << "Error:" << reply->errorString();
        }

        reply->deleteLater();
        QCoreApplication::quit();
    });

    return a.exec();
}


*/
