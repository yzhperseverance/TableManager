#ifndef STAFFMANAGE_H
#define STAFFMANAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QString>
#include <QList>
#include <QScreen>
#include "database.h"
#include "permission.h"
#include "register.h"
#include "config.h"
#include "mymessagebox.h"
namespace Ui {
class staffManage;
}

class StaffManage : public QWidget
{
    Q_OBJECT

public:
    explicit StaffManage(QWidget *parent = 0);

    void on_tablewidget_cellChanged(int row,int col);

    void on_Regist_registComplete();

    void openWindow(QString name, QString permiss, bool selfWindow);

    void openWindow(QString name, QString permiss);

    void initTable();

    void showStaffInfo();

    void showSelfInfo();

    void deleteItem(int row, int col);

    ~StaffManage();

private slots:
    void on_addButton_clicked();

    void on_deleteButton_clicked();

    void on_uploadButton_clicked();

signals:
    void nameChangeSignal(QString name);


private:
    Ui::staffManage *ui;
    QString permission;
    QString name;
    UserInfo userInfo;
    UserPermissionManager userPermissionManager;
    Register *regist;
    CommunicateWithServer server;
    bool self = false;
};

#endif // STAFFMANAGE_H
