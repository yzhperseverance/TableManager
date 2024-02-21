#ifndef PEIMISSION_H
#define PEIMISSION_H

#include <QString>
// 管理用户权限


class UserPermissionManager
{

public:

    QStringList getAllAvailablePermission() const {
        QStringList ret;
        ret << administrator << staff;
        return ret;
    }

    const QString super ="超级管理员";
    const QString administrator = "管理员";
    const QString staff = "普通用户";



};





#endif // PEIMISSION_H
