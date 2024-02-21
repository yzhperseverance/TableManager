#include "config.h"

namespace config {

const QString config_path = "config.json";

QString table_url;
QString user_url;
QString table_path;
QString user_info_path;
QString nextcloud_url;
QString nextcloud_username;
QString nextcloud_password;


    void readConfigFile(){
        QFile file(config_path);
        if (!file.open(QIODevice::ReadOnly)) {
            return;
        }

        // 读取JSON文件内容
        QByteArray jsonData = file.readAll();
        file.close();

        // 解析JSON数据
        QJsonParseError error;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &error);
        if (error.error != QJsonParseError::NoError) {
            return;
        }

        if (!jsonDoc.isObject()) {
            return;
        }

        // 将JSON数据转换为对象
        QJsonObject jsonObj = jsonDoc.object();

        if (jsonObj.contains("table_url") && jsonObj["table_url"].isString()) {
            table_url = jsonObj["table_url"].toString();
        }

        if (jsonObj.contains("user_url") && jsonObj["user_url"].isString()) {
            user_url = jsonObj["user_url"].toString();
        }

        if (jsonObj.contains("table_path") && jsonObj["table_path"].isString()) {
            table_path = jsonObj["table_path"].toString();
        }

        if (jsonObj.contains("user_info_path") && jsonObj["user_info_path"].isString()) {
            user_info_path = jsonObj["user_info_path"].toString();
        }

        if (jsonObj.contains("nextcloud_url") && jsonObj["nextcloud_url"].isString()) {
            nextcloud_url = jsonObj["nextcloud_url"].toString();
        }

        if (jsonObj.contains("nextcloud_username") && jsonObj["nextcloud_username"].isString()) {
            nextcloud_username = jsonObj["nextcloud_username"].toString();
        }

        if (jsonObj.contains("nextcloud_password") && jsonObj["nextcloud_password"].isString()) {
            nextcloud_password = jsonObj["nextcloud_password"].toString();
        }
    }



}
