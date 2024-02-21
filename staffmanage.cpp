#include "staffmanage.h"
#include "ui_staffmanage.h"

StaffManage::StaffManage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::staffManage)
{
    ui->setupUi(this);
    regist = new Register();
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &StaffManage::on_tablewidget_cellChanged);
    connect(regist, &Register::registComplete, this, &StaffManage::on_Regist_registComplete);
    ui->addButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->deleteButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->tableWidget->horizontalHeader()->setStyleSheet("font-family:'Arial'; font-size: 10pt; font-weight: bold;");
    ui->uploadButton->setStyleSheet("background-color:#69b3f0;; font-family:'Arial'; font-size: 10pt");

    // 获取当前屏幕分辨率
    const QSize screenSize = qApp->primaryScreen()->size();
    const int screenWidth = screenSize.width();
    const int screenHeight = screenSize.height();
    // 设置缩放比例
    qreal scaleX = static_cast<qreal>(screenWidth) / 1920.0;  // 根据自己期望的基准分辨率调整，这里以1920x1080为例
    qreal scaleY = static_cast<qreal>(screenHeight) / 1080.0;
    this->resize(749 * scaleX, 516 * scaleY);  // 设置固定宽度和高度

}

StaffManage::~StaffManage()
{
    delete ui;
}

void StaffManage::openWindow(QString name, QString permiss, bool selfWindow){
    initTable();
    this->name = name;
    this->permission = permiss;
    this->self = true;
    this->setWindowTitle("我的资料");
    QStringList header;
    ui->tableWidget->setColumnCount(3);
    header << "姓名" << "密码" << "权限";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    showSelfInfo();
    this->show();
}

void StaffManage::openWindow(QString name, QString permiss){
    this->name = name;
    this->permission = permiss;
    this->self = false;
    this->setWindowTitle("所有用户资料");
    initTable();
    QStringList header;
    if(permission == userPermissionManager.super){
        ui->tableWidget->setColumnCount(3);
        header << "姓名" << "密码" << "权限";
    }
    else if(permission == userPermissionManager.administrator){
        ui->tableWidget->setColumnCount(2);
        header << "姓名" << "权限";
    }
    ui->tableWidget->setHorizontalHeaderLabels(header);
    showStaffInfo();

    this->show();
}

void StaffManage::initTable(){
    // 设置列宽不能被手动拉伸，要放在设置列宽前
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui->tableWidget->horizontalHeader()->setFont(QFont("微软雅黑", 20));
    // 由于后四项字比较少，所以让后四项根据item调整宽度，前两项自适应
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置表头中间对齐
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    // 设置奇偶行颜色区分
    ui->tableWidget->setAlternatingRowColors(true);
    // 设置窗口置顶
    ui->tableWidget->raise();
    ui->tableWidget->activateWindow();

}

void StaffManage::showSelfInfo(){
    ui->tableWidget->blockSignals(true);


    int ncount = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(0);

    QStringList self_info;
    self_info = userInfo.getSelfInfo(this->name);
    ui->tableWidget->insertRow(0);
    for(int i = 0; i < self_info.size(); i++){
        QTableWidgetItem *item = new QTableWidgetItem(self_info[i]);
        item->setFont(QFont("微软雅黑", 10));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(0, i, item);
        // 不可编辑自己的权限
        if(i == 2){
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }

    }


    ui->tableWidget->blockSignals(false);
}

void StaffManage::showStaffInfo(){
    ui->tableWidget->blockSignals(true);
    QList<QStringList> staff_info;

    int ncount = ui->tableWidget->rowCount();

    ui->tableWidget->setRowCount(0);
    if(permission == userPermissionManager.super){
        staff_info = userInfo.getAllInfo();
    }
    else if(permission == userPermissionManager.administrator){
        staff_info = userInfo.getAllNamePerimission();
    }


    for(int row = 0; row < staff_info.size(); row++){
        ui->tableWidget->insertRow(row);
        for(int i = 0; i < staff_info[row].size(); i++){
            QTableWidgetItem *item = new QTableWidgetItem(staff_info[row][i]);
            item->setFont(QFont("微软雅黑", 10));
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableWidget->setItem(row, i, item);
            if(permission == userPermissionManager.super){
                // 密码只有本人可以编辑
                if(i == 1 || (i == 2 && staff_info[row][2] == userPermissionManager.super)){
                  item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }

            }
            else if(permission == userPermissionManager.administrator){
                // 管理员只能编辑普通用户的名称
                if(i == 1 || staff_info[row][1] == userPermissionManager.super || staff_info[row][1] == userPermissionManager.administrator){
                    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                }
            }
        }

    }
    ui->tableWidget->blockSignals(false);
}


void StaffManage::deleteItem(int row, int col){
    QTableWidgetItem *item = ui->tableWidget->takeItem(row, col);
    delete item;
}


void StaffManage::on_tablewidget_cellChanged(int row,int col){
    ui->tableWidget->blockSignals(true);
    // 如果修改的是名字，则需要从json文件中获取原名字
    if(col == 0){
        QString old_name = userInfo.getIndexInfo(row)[0];
        QString new_name = ui->tableWidget->item(row, col)->text();
        if(userInfo.findData(new_name)){
            QMessageBox *no_name;
            no_name->warning(this, "警告", "用户名重复！");
        }
        else{
            userInfo.modifyData(old_name, new_name, 0);
            ui->tableWidget->blockSignals(false);
            emit nameChangeSignal(new_name);
        }

    }
    else if(col == 1 && self){
        QString name = ui->tableWidget->item(row, 0)->text();
        QString password = ui->tableWidget->item(row, col)->text();
        if(password.size() < 6){
            QMessageBox *no_name;
            no_name->warning(this, "警告", "密码至少为6位！");
        }
        else{
            userInfo.modifyData(name, password, 1);
        }
    }
    // 修改权限
    else if(col == 2 && !self){
        QString name = ui->tableWidget->item(row, 0)->text();
        QString permission = ui->tableWidget->item(row, col)->text();
        if(permission == userPermissionManager.super){
            QMessageBox *no_name;
            no_name->warning(this, "警告", "无法创建超级管理员权限！");
        }
        else if(permission != userPermissionManager.administrator && permission != userPermissionManager.staff){
            QMessageBox *no_name;
            no_name->warning(this, "警告", "无效权限！");
        }
        else{
           userInfo.modifyData(name, permission, 2);
        }
    }
    ui->tableWidget->blockSignals(false);
}


void StaffManage::on_addButton_clicked()
{
    QMessageBox *no_name;
    if(permission == userPermissionManager.staff){
        no_name->warning(this, "警告", "无权限！");
    }
    else if(this->self){
        no_name->warning(this, "警告", "请在“管理用户”界面添加！");
    }
    else{
        regist->openWindow(this->permission);
    }
}

void StaffManage::on_Regist_registComplete(){
    showStaffInfo();
}


void StaffManage::on_deleteButton_clicked()
{
    QMessageBox *no_name;
    if(permission == userPermissionManager.staff){
        no_name->warning(this, "警告", "无权限！");
        return;
    }
    else if(this->self){
        no_name->warning(this, "警告", "请在“管理用户”界面删除！");
        return;
    }

    QList<QTableWidgetSelectionRange> indexList = ui->tableWidget->selectedRanges();
    if(indexList.isEmpty()){
        no_name->warning(this, "警告", "请选中要删除的员工姓名！");
        return;
    }

    int startRow = indexList.first().topRow();
    int endRow = indexList.last().bottomRow();
    int startCol = indexList.first().leftColumn();
    int endCol = indexList.last().rightColumn();
    if(startCol != 0 || endCol != 0){
        no_name->warning(this, "警告", "请选中要删除的员工姓名！");
        return;
    }

    for(int i = startRow;i <= endRow;i++)
    {
        QString name = ui->tableWidget->item(i, 0)->text();
        QString permis = ui->tableWidget->item(i, ui->tableWidget->columnCount() - 1)->text();
        if(permis == userPermissionManager.super){
            no_name->warning(this, "警告", "无法删除超级管理员！");
            return;
        }
        else if(name == this->name){
            no_name->warning(this, "警告", "无法删除自己！");
            return;
        }
        else if(permis == userPermissionManager.administrator && this->permission == userPermissionManager.administrator){
            no_name->warning(this, "警告", "无权限！");
            return;
        }
        else{
            userInfo.deleteData(name);
        }

    }
    no_name->information(this, "提示", "删除成功！");
    showStaffInfo();
}

void StaffManage::on_uploadButton_clicked()
{
    QMessageBox *no_name;
    server.uploadFileToNextcloud(config::user_info_path, config::nextcloud_url, config::nextcloud_username, config::nextcloud_password);
    no_name->information(this, "提示", "上传成功！");
}
