#include "login.h"
#include "ui_login.h"

login::login(Widget *widget, QWidget *parent) :
    widget(widget),
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    QMovie *m_movie = new QMovie("../peach/icons/login.gif");

    ui->backgroundLabel->setScaledContents(true);				// 设置标签为可扩展内容
    ui->backgroundLabel->setMovie(m_movie);
    ui->backgroundLabel->lower();
    m_movie->start();

    this->setWindowTitle("船舶证书处理系统");

    // 限制窗口缩放
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
    // 仅能使用英文输入法
    //ui->accountEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->passwordEdit->setAttribute(Qt::WA_InputMethodEnabled, false);

    // 取消按钮的tab键响应
    ui->loginButton->setFocusPolicy(Qt::NoFocus);

    server.downloadUserInfo();
}

login::~login()
{
    delete ui;
}

// 不断刷新，让按钮在指定的位置上
void login::paintEvent(QPaintEvent*){
    ui->backgroundLabel->resize(this->size());

   int width = this->width(), height = this->height();
}

// 获取用户的输入
void login::on_accountEdit_editingFinished()
{
    this->username = ui->accountEdit->text();
}

void login::on_passwordEdit_editingFinished()
{
    this->password = ui->passwordEdit->text();
}


// 回车键响应
void login::keyPressEvent(QKeyEvent *event){
    QMessageBox *no_name;
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(this->username.isEmpty()){
            no_name->warning(this, "警告", "请输入用户名!");
        }
        else if(this->password.isEmpty()){
            no_name->warning(this, "警告", "请输入密码!");
        }
        else{
            QString perimission;
            // 用户名和密码正确
            if(userinfo.findData(this->username, this->password, perimission)){
                widget->openWindow(this->username, perimission);
                this->close();
            }
            else{
                no_name->warning(this, "警告", "用户名或密码错误!");
            }
        }
    }

}

void login::on_loginButton_clicked()
{
    // 这里还需要再赋值，因为如果只点击按钮但光标还在文本框中时，不会触发editfinished信号
    this->username = ui->accountEdit->text();
    this->password = ui->passwordEdit->text();
    QMessageBox *no_name;
    if(this->username.isEmpty()){
        no_name->warning(this, "警告", "请输入用户名!");
    }
    else if(this->password.isEmpty()){
        no_name->warning(this, "警告", "请输入密码!");
    }
    else{
        QString perimission;
        // 用户名和密码正确
        if(userinfo.findData(this->username, this->password, perimission)){ 
            widget->openWindow(this->username, perimission);
            this->close();
        }
        else{
            no_name->warning(this, "警告", "用户名或密码错误!");
        }
    }

}




