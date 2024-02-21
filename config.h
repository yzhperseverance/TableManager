#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#

namespace config{

extern const QString config_path;

extern QString table_url;
extern QString user_url;
extern QString table_path;
extern QString user_info_path;
extern QString nextcloud_url;
extern QString nextcloud_username;
extern QString nextcloud_password;

void readConfigFile();




}

#endif // CONFIG_H
