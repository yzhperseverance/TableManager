#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMovie>
#include <QSize>
#include <QString>
#include <QMessageBox>
#include <QKeyEvent>
#include "widget.h"
#include "rules.h"
#include "database.h"
#include "register.h"
namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(Widget *widget, QWidget *parent = 0);

    void paintEvent(QPaintEvent*) override;

    void keyPressEvent(QKeyEvent *event);

    ~login();


signals:
    void enterSignal();

private slots:
    void on_accountEdit_editingFinished();

    void on_passwordEdit_editingFinished();


    void on_loginButton_clicked();

private:
    Ui::login *ui;
    Widget *widget;

    QString username;
    QString password;
    UserInfo userinfo;
    TextInputRule rule;
    CommunicateWithServer server;

};

#endif // LOGIN_H
