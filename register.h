#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>
#include <QString>
#include <permission.h>
#include <QKeyEvent>
#include <QCloseEvent>
#include <QMessageBox>
#include <QScreen>
#include "rules.h"
#include "database.h"
#include <iostream>
using namespace std;

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = 0);
    ~Register();

    void openWindow(QString permission);

    void keyPressEvent(QKeyEvent *event);

    void closeEvent(QCloseEvent *event);

private slots:

    void on_usernameEdit_editingFinished();

    void on_passwordEdit_editingFinished();

    bool checkRule();

    void on_registerButton_clicked();

    void on_permissionBox_activated(const QString &arg1);

signals:
    void registComplete();

private:
    Ui::Register *ui;
    QString username;
    QString password;
    QString permission;
    TextInputRule rule;
    UserInfo user;
    UserPermissionManager userPermissionManager;

    bool is_open = false;

public:
    QString cur_permission;

};

#endif // REGISTER_H
