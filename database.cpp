#include "database.h"





void CommunicateWithServer::downloadTable(){
    QNetworkAccessManager manager;
    QNetworkRequest request(config::table_url);
    QNetworkReply *reply = manager.get(request);

    // 使用QFile保存下载的文件
    QFile file(config::table_path);
    if (file.open(QIODevice::WriteOnly)) {
        QObject::connect(reply, &QNetworkReply::readyRead, [&]() {
            file.write(reply->readAll());
        });

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            file.close();
            loop.quit();
            reply->deleteLater();
        });

        loop.exec();

    } else {
        qDebug() << "Unable to open file for writing.";
    }

}


void CommunicateWithServer::downloadUserInfo(){
    QNetworkAccessManager manager;
    QNetworkRequest request(config::user_url);
    QNetworkReply *reply = manager.get(request);

    // 使用QFile保存下载的文件
    QFile file(config::user_info_path);
    if (file.open(QIODevice::WriteOnly)) {
        QObject::connect(reply, &QNetworkReply::readyRead, [&]() {
            file.write(reply->readAll());
        });

        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, [&]() {
            file.close();
            loop.quit();
            reply->deleteLater();
        });

        loop.exec();

    } else {
        qDebug() << "Unable to open file for writing.";
    }

}


void CommunicateWithServer::uploadFileToNextcloud(const QString &localFilePath, const QString &nextcloudUrl, const QString &username, const QString &password) {

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl(nextcloudUrl + "/remote.php/dav/files/" + username + "/" + QFileInfo(localFilePath).fileName()));
    request.setRawHeader("Authorization", "Basic " + QByteArray(QString("%1:%2").arg(username).arg(password).toLatin1().toBase64()));

    QFile file(localFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file for reading";
        return;
    }

    QNetworkReply* reply = manager.put(request, &file);

    QEventLoop loop;
    QObject::connect(reply, &QNetworkReply::finished, [&]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Upload successful!";
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        loop.quit();
        reply->deleteLater();
    });
    loop.exec();
}


void UserInfo::addData(QString Qname, QString Qpassword, QString Qpermission){

    QFile file(config::user_info_path);
    if(!file.open(QIODevice::ReadWrite)) {
       qDebug() << "File open error";
       return;
    }
    else{
       qDebug() <<"File open!";
    }

    QByteArray jsonData=file.readAll();//读取Json文件内容
    QJsonParseError error;    //解析错误反馈
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
    if(error.error==QJsonParseError::NoError){
        QJsonObject obj=doc.object(); //Json键值对

        QJsonArray subarray = obj.value("array").toArray();

        string name = Qname.toStdString();
        string password = Qpassword.toStdString();
        string permission = Qpermission.toStdString();
        QJsonObject jsonObject;
        jsonObject.insert("name", name.c_str());
        jsonObject.insert("password", password.c_str());
        jsonObject.insert("permission", permission.c_str());

        subarray.append(jsonObject);
        obj["array"] = subarray;

        QJsonDocument jsonDoc(obj);
        file.resize(0);
        file.write(jsonDoc.toJson());
        file.close();

        qDebug() << "Write to file";
    }


}
void UserInfo::deleteData(QString name){
    QFile file(config::user_info_path);
    if(!file.open(QIODevice::ReadWrite)) {
       qDebug() << "File open error";
       return;
    }
    else{
       qDebug() <<"File open!";
    }

    QByteArray jsonData=file.readAll();//读取Json文件内容
    QJsonParseError error;    //解析错误反馈
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
    if(error.error==QJsonParseError::NoError){
        QJsonObject doc_obj=doc.object(); //Json键值对

        QJsonArray subarray = doc_obj.value("array").toArray();
        for(int i = 0; i < subarray.size(); i++) {
           if (subarray[i].isObject()) {
               QJsonObject obj = subarray[i].toObject();
               auto it = obj.find("name");
               if(it != obj.end() && it.value() == name){
                   subarray.removeAt(i);
                   doc_obj["array"] = subarray;
                   doc.setObject(doc_obj);
                   file.resize(0);
                   file.write(doc.toJson());
                   file.close();
               }

           }

        }
    }


}


 bool UserInfo::findData(QString Qname){
     QFile file(config::user_info_path);
     if(file.open(QIODevice::ReadOnly)){
         QByteArray jsonData=file.readAll();//读取Json文件内容
         file.close();//关闭文件IO
         QJsonParseError error;    //解析错误反馈
         QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
         if(error.error==QJsonParseError::NoError){

             QJsonObject obj=doc.object(); //Json键值对
             QJsonArray subarray = obj.value("array").toArray();

             for(const auto& val : subarray) {

                if (val.isObject()) {
                // 如果元素是对象，则输出其所有属性的键
                QJsonObject obj = val.toObject();
                //qDebug() << obj;
                auto it = obj.find("name");
                if(it != obj.end() && it.value() == Qname) return true;

                }

             }
            return false;
        }
     }
     else{
         qDebug() << "File open error";
     }

    return false;

 }

 bool UserInfo::findData(QString Qname, QString Qpassword, QString &permission){
     QFile file(config::user_info_path);
     if(file.open(QIODevice::ReadOnly)){
         QByteArray jsonData=file.readAll();//读取Json文件内容
         file.close();//关闭文件IO
         QJsonParseError error;    //解析错误反馈
         QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
         if(error.error==QJsonParseError::NoError){

             QJsonObject obj=doc.object(); //Json键值对
             QJsonArray subarray = obj.value("array").toArray();

             for(const auto& val : subarray) {

                if (val.isObject()) {
                    QJsonObject obj = val.toObject();
                    auto it = obj.find("name");
                    if(it != obj.end() && it.value() == Qname && obj.find("password").value() == Qpassword){
                        permission = obj.find("permission").value().toString();
                        return true;
                    }

                }

             }
        }

     }
     else{
         qDebug() << "File open error";
     }

     return false;

 }




void UserInfo::modifyData(QString name, QString info, int symbol){
    QFile file(config::user_info_path);
    if(!file.open(QIODevice::ReadWrite)) {
       qDebug() << "File open error";
       return;
    }
    else{
       qDebug() <<"File open!";
    }

    QByteArray jsonData=file.readAll();//读取Json文件内容
    QJsonParseError error;    //解析错误反馈
    QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
    if(error.error==QJsonParseError::NoError){

        QJsonObject doc_obj=doc.object(); //Json键值对
        QJsonArray subarray = doc_obj.value("array").toArray();

        for(int i = 0; i < subarray.size(); i++) {

           if (subarray[i].isObject()) {
               QJsonObject obj = subarray[i].toObject();
               auto it = obj.find("name");
               if(it != obj.end() && it.value() == name)
               {
                   if(symbol == 0){
                       obj["name"] = info;
                   }
                   else if(symbol == 1){
                       obj["password"] = info;
                   }
                   else if(symbol == 2){
                       obj["permission"] = info;
                   }
                   subarray.replace(i, obj);
                   doc_obj["array"] = subarray;
                   doc.setObject(doc_obj);
                   file.resize(0);
                   file.write(doc.toJson());
                   file.close();
               }

           }

        }
   }

}


QList<QStringList> UserInfo::getAllNamePerimission(){
    QFile file(config::user_info_path);
    QList<QStringList> ret;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray jsonData=file.readAll();//读取Json文件内容
        file.close();//关闭文件IO
        QJsonParseError error;    //解析错误反馈
        QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
        if(error.error==QJsonParseError::NoError){
            QJsonObject obj=doc.object(); //Json键值对
            QJsonArray subarray = obj.value("array").toArray();

            for(const auto& val : subarray) {
               QStringList info;
               if (val.isObject()) {
                   QJsonObject obj = val.toObject();
                   auto it = obj.find("name");
                   if(it != obj.end()){
                       info.append(it.value().toString());
                   }
                   it = obj.find("permission");
                   if(it != obj.end()){
                       info.append(it.value().toString());
                   }
               }
               ret.append(info);
            }
       }

    }
    else{
        qDebug() << "File open error";
    }

    return ret;
}


QList<QStringList> UserInfo::getAllInfo(){
    QFile file(config::user_info_path);
    QList<QStringList> ret;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray jsonData=file.readAll();//读取Json文件内容
        file.close();//关闭文件IO
        QJsonParseError error;    //解析错误反馈
        QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
        if(error.error==QJsonParseError::NoError){
            QJsonObject obj=doc.object(); //Json键值对
            QJsonArray subarray = obj.value("array").toArray();

            for(const auto& val : subarray) {
               QStringList info;
               if (val.isObject()) {
                   QJsonObject obj = val.toObject();
                   auto it = obj.find("name");
                   if(it != obj.end()){
                       info.append(it.value().toString());
                   }
                   it = obj.find("password");
                   if(it != obj.end()){
                       info.append(it.value().toString());
                   }
                   it = obj.find("permission");
                   if(it != obj.end()){
                       info.append(it.value().toString());
                   }
               }
               ret.append(info);
            }
       }

    }
    else{
        qDebug() << "File open error";
    }

    return ret;

}


QStringList UserInfo::getIndexInfo(int index){
    QFile file(config::user_info_path);
    QStringList ret;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray jsonData=file.readAll();//读取Json文件内容
        file.close();//关闭文件IO
        QJsonParseError error;    //解析错误反馈
        QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
        if(error.error==QJsonParseError::NoError){
            QJsonObject obj=doc.object(); //Json键值对
            QJsonArray subarray = obj.value("array").toArray();
            if(index >= subarray.size() || index < 0) return ret;
            QJsonObject user_obj = subarray.at(index).toObject();

           auto it = user_obj.find("name");
           if(it != user_obj.end()){
               ret.append(it.value().toString());
           }
           it = user_obj.find("password");
           if(it != user_obj.end()){
               ret.append(it.value().toString());
           }
           it = user_obj.find("permission");
           if(it != user_obj.end()){
               ret.append(it.value().toString());
           }


       }

    }
    else{
        qDebug() << "File open error";
    }


    return ret;
}



QStringList UserInfo::getSelfInfo(QString name){
    QFile file(config::user_info_path);
    QStringList ret;
    if(file.open(QIODevice::ReadOnly)){
        QByteArray jsonData=file.readAll();//读取Json文件内容
        file.close();//关闭文件IO
        QJsonParseError error;    //解析错误反馈
        QJsonDocument doc=QJsonDocument::fromJson(jsonData,&error);
        if(error.error==QJsonParseError::NoError){

            QJsonObject obj=doc.object(); //Json键值对
            QJsonArray subarray = obj.value("array").toArray();

            for(const auto& val : subarray) {

               if (val.isObject()) {
                   QJsonObject obj = val.toObject();
                   auto it = obj.find("name");
                   if(it != obj.end() && it.value() == name){
                       QString password = obj.find("password").value().toString();
                       QString permission = obj.find("permission").value().toString();
                       ret << name << password << permission;
                       break;
                   }

               }

            }
       }

    }
    else{
        qDebug() << "File open error";
    }
    return ret;

}

