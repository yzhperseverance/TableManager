#include "register.h"
#include "ui_register.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);

    // 设置仅能使用英文输入法
    //ui->usernameEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->passwordEdit->setAttribute(Qt::WA_InputMethodEnabled, false);
    ui->permissionBox->setEditable(false);

    // 取消按钮的tab键响应
    ui->registerButton->setFocusPolicy(Qt::NoFocus);

    ui->label_2->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt;");
    ui->label_3->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt;");
    ui->label_4->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt;");
    ui->usernameEdit->setStyleSheet("font-family:'Arial'; font-size: 10pt");
    ui->passwordEdit->setStyleSheet("font-family:'Arial'; font-size: 10pt");
    ui->permissionBox->setStyleSheet("font-family:'Arial'; font-size: 10pt");
    ui->registerButton->setStyleSheet("font-family:Microsoft YaHei UI;font-size:12pt;");




    // 获取当前屏幕分辨率
    const QSize screenSize = qApp->primaryScreen()->size();
    const int screenWidth = screenSize.width();
    const int screenHeight = screenSize.height();
    // 设置缩放比例
    qreal scaleX = static_cast<qreal>(screenWidth) / 1920.0;  // 根据自己期望的基准分辨率调整，这里以1920x1080为例
    qreal scaleY = static_cast<qreal>(screenHeight) / 1080.0;
    this->resize(375 * scaleX, 500 * scaleY);
    ui->frame->resize(375 * scaleX, 500 * scaleY);

    ui->label_3->move(110 * scaleX, 90 * scaleY);
    ui->label_2->move(110 * scaleX, 170 * scaleY);
    ui->label_4->move(110 * scaleX, 260 * scaleY);

    ui->usernameEdit->move(110 * scaleX, 130 * scaleY);
    ui->passwordEdit->move(110 * scaleX, 210 * scaleY);
    ui->permissionBox->move(110 * scaleX, 300 * scaleY);
    ui->registerButton->move(90 * scaleX, 420 * scaleY);
}

Register::~Register()
{
    delete ui;
}


void Register::openWindow(QString permission){
    this->setWindowTitle("注册");
    cur_permission = permission;
    if(is_open == false){
        if(cur_permission == userPermissionManager.super){
            ui->permissionBox->addItems(userPermissionManager.getAllAvailablePermission());
            this->permission = userPermissionManager.administrator;
        }
        else if(cur_permission == userPermissionManager.administrator){
            ui->permissionBox->addItem(userPermissionManager.staff);
            this->permission = userPermissionManager.staff;
        }
        is_open = true;
    }

    ui->frame->raise();
    ui->frame->activateWindow();
    this->show();
}

void Register::closeEvent(QCloseEvent *event){
    is_open = false;
    username = "";
    password = "";
    permission = "";
    ui->usernameEdit->clear();
    ui->passwordEdit->clear();
}

void Register::on_usernameEdit_editingFinished()
{
    this->username = ui->usernameEdit->text();
}

void Register::on_passwordEdit_editingFinished()
{
    this->password = ui->passwordEdit->text();
}

void Register::on_permissionBox_activated(const QString &arg)
{
    this->permission = arg;
}

bool Register::checkRule(){
    bool bret = false;
    QMessageBox *no_name;
    // 用户未输入用户名
    if(this->username.isEmpty()){
        no_name->warning(this, "警告", "请输入用户名！");
    }
    else if(this->password.isEmpty()){
        no_name->warning(this, "警告", "请输入密码！");
    }
    else if(this->permission.isEmpty()){
        no_name->warning(this, "警告", "请输入权限！");
    }
    else if(this->password.length() < 6){
        no_name->warning(this, "警告", "密码应至少为6位字符！");
    }
    // 检查用户名是否重复
    else if(user.findData(this->username)){
        no_name->warning(this, "警告", "用户名重复！");
    }
    else{
        bret = true;
    }
    return bret;


}

void Register::keyPressEvent(QKeyEvent *event){

    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return){
        if(checkRule()){
            user.addData(this->username, this->password, this->permission);
            QMessageBox *no_name;
            no_name->information(this, "提示", "注册成功！");
            emit registComplete();
        }
    }

}


void Register::on_registerButton_clicked()
{
    // 这里还需要再赋值，因为如果只点击按钮但光标还在文本框中时，不会触发editfinished信号
    this->username = ui->usernameEdit->text();
    this->password = ui->passwordEdit->text();

    if(checkRule()){
        user.addData(this->username, this->password, this->permission);
        QMessageBox *no_name;
        no_name->information(this, "提示", "注册成功！");
        emit registComplete();
    }
}


