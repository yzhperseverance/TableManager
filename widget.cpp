#include "widget.h"

//#include <QGraphicsDropShadowEffect>







Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(ui->tableWidget1, &QTableWidget::cellChanged, this, &Widget::on_tablewidget_cellChanged);
    connect(ui->tableWidget2, &QTableWidget::cellChanged, this, &Widget::on_tablewidget_cellChanged);
    manageSheet = new ManageSheet();
    connect(manageSheet, &ManageSheet::addsheetSignal, this, &Widget::on_managesheet_addsheet);
    connect(manageSheet, &ManageSheet::deletesheetSignal, this, &Widget::on_managesheet_deletesheet);
    staffManage = new StaffManage();
    connect(staffManage, &StaffManage::nameChangeSignal, this, &Widget::on_staffManage_nameChange);

    // 要先注册才能使用过滤器
    ui->w_bg->installEventFilter(this);



}

Widget::~Widget()
{
    delete ui;
}

/**********************************************************初始化函数****************************************************************/

void Widget::initUi()
{
    //初始化窗口边框
    //this->setAttribute(Qt::WA_TranslucentBackground, true);
    //this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(QColor("#444444"));
    shadow->setBlurRadius(16);
    //ui->w_bg->setGraphicsEffect(shadow);
    ui->lay_bg->setMargin(12);

    //Logo
    QPixmap logo(":/icons/logo.png");
    ui->lab_logo->setPixmap(logo);

    //itemLogo
    QPixmap item1(":/icons/item_01.png");
    QPixmap item2(":/icons/item_02.png");
    QPixmap item3(":/icons/modular.png");
//    ui->lab_item_01->setPixmap(item1);
//    ui->lab_item_02->setPixmap(item2);
    ui->lab_item_03->setPixmap(item3);

    this->setWindowTitle("船舶证书处理系统");


    // 获取当前屏幕分辨率
    const QSize screenSize = qApp->primaryScreen()->size();
    const int screenWidth = screenSize.width();
    const int screenHeight = screenSize.height();
    // 设置缩放比例
    qreal scaleX = static_cast<qreal>(screenWidth) / 2560.0;  // 根据自己期望的基准分辨率调整，这里以1920x1080为例
    qreal scaleY = static_cast<qreal>(screenHeight) / 1600.0;
    this->resize(1525 * scaleX, 1128 * scaleY);

}

void Widget::initMember()
{
    this->myMbox = new MyMessageBox();
    // 绑定对话框输出与对应槽函数
    connect(myMbox,SIGNAL(btnchicked(int)),this,SLOT(changeMboxReturn(int)));

    //最小化到托盘
    QIcon icon = QIcon(":/icons/logo.png");//设置最小图标
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("Peach"); //提示文字
    //添加托盘列表项(还原与退出)
    returnNormal = new QAction(" Show", this);
    returnNormal->setFont(QFont("Arial", 9));
    returnNormal->setObjectName("returnNormal");
    returnNormal->setIcon(QIcon(":/icons/show.png"));
    quitAction = new QAction(" Quit", this);
    quitAction->setFont(QFont("Arial", 9));
    quitAction->setObjectName("quitAction");
    quitAction->setIcon(QIcon(":/icons/out.png"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));           //绑定槽函数退出
    connect(returnNormal, SIGNAL(triggered()), this, SLOT(showNormal()));   //绑定槽函数还原界面

    //创建托盘菜单(必须先创建动作，后添加菜单项，还可以加入菜单项图标美化)
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(returnNormal);
    trayIconMenu->addAction(quitAction);
    trayIcon->setContextMenu(trayIconMenu);
}

void Widget::initTable()
{
    //TODO:所有路径都应该写在配置文件里

    xlsx = std::make_shared<QXlsx::Document>(config::table_path);

    QStringList ship_type = tableManager.getShipType(xlsx);
    ui->ShipTypecomboBox->clear();
    ui->ShipTypecomboBox->addItems(ship_type);
    // 设置按钮风格
    ui->waitingButton->setStyleSheet("background-color:#AAAAAA; font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->allButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->refreshButton->setStyleSheet("background-color:#69b3f0;font-family:'Microsoft YaHei UI'; font-size: 11pt");
    ui->uploadButton->setStyleSheet("background-color:#69b3f0;font-family:'Microsoft YaHei UI'; font-size: 11pt");
    ui->staffManageButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->manageSheetButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->pullLocalButton->setStyleSheet("background-color:#69b3f0; font-family:'Microsoft YaHei UI'; font-size: 10pt");
    ui->ShipTypecomboBox->setStyleSheet("font-family:'Arial'; font-size: 12pt");

    for(int i = 0; i <= 1; i++){

        QTableWidget *tableWidget = ui->stackedWidget->widget(i)->findChildren<QTableWidget*>()[0];
        tableWidget->setColumnCount(tableManager.col_num);
        QStringList header;
        header << "中文名称" << "英文名称" << "数量" << "签发机构" << "到期时间" << "状态";
        tableWidget->setHorizontalHeaderLabels(header);
        // 设置列宽不能被手动拉伸，要放在设置列宽前
        tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        // 由于后四项字比较少，所以让后四项根据item调整宽度，前两项自适应
        tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
        tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
        tableWidget->horizontalHeader()->setStyleSheet("font-family:'Arial'; font-size: 10pt; font-weight: bold;");
        // 设置表头中间对齐
        tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
        // 设置奇偶行颜色区分
        tableWidget->setAlternatingRowColors(true);

        // 初始化表格
        QList<QStringList> certififate_info = tableManager.getAllTargetCeritificate(xlsx, tableWidget);
        showTabelInfo(certififate_info, tableWidget);
    }
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTableWidget::customContextMenuRequested, this, &Widget::on_tableWidget_customContextMenuRequested);


}

/**********************************************************自定义函数****************************************************************/



void Widget::littleShow()
{
    this->hide();//隐藏主窗口
    trayIcon->show();//显示托盘

    //显示到系统提示框的信息
    QString title="Peach";
    QString text="正自动在后台运行";
    trayIcon->showMessage(title,text,QSystemTrayIcon::Information,3000); //此参数为提示时长
}

void Widget::insertTabelItems(int row, int start_col, int end_col, QStringList items, QTableWidget *current_table){
    current_table->blockSignals(true);
    for(int i = start_col; i <= end_col; i++){
        int item_index = i - start_col;
        QTableWidgetItem *item = new QTableWidgetItem(items[item_index]);
        if(i == tableManager.col_num - 1){
            if(items[item_index] == tableManager.waiting){
                item->setTextColor(QColor(255,0,0));
            }
            else if(items[item_index] == tableManager.ignore){
                item->setTextColor(QColor(0,0,255));
            }
            else if(items[item_index] == tableManager.finished){
            }
        }
        item->setFont(QFont("微软雅黑", 10));
        item->setTextAlignment(Qt::AlignCenter);
        // 超级管理员可以修改一切数据
        if(this->permission == userPermissionManager.super){}
        else{
            if(i != tableManager.col_num - 1){
                item->setFlags(item->flags() & (~Qt::ItemIsEditable));
            }
            else{
                if(this->permission == userPermissionManager.staff){
                    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
                }
            }
        }

        current_table->setItem(row, i, item);
    }
    current_table->blockSignals(false);
}

void Widget::deleteItem(int row, int col){
    QTableWidgetItem *item = getCurrentTableWidget()->takeItem(row, col);
    delete item;
}


// 这里的cur_table参数不能删掉直接调用getCurrentTableWidget，因为初始化的时候也要用到
void Widget::showTabelInfo(QList<QStringList> certififate_info, QTableWidget *current_table){
    current_table->blockSignals(true);
    current_table->setRowCount(0);
    for(int i = 0; i < certififate_info.size(); i++){
        if(certififate_info[i].size() == tableManager.col_num){
            current_table->insertRow(i);
            insertTabelItems(i, 0, tableManager.col_num - 1, certififate_info[i], current_table);
        }
        else{
            // 固定读6列，空行也是
            if(certififate_info[i].size() <= 1) continue;

            int row = certififate_info[i][certififate_info[i].size() - 1].toInt(); // 最后一项是该数据所在行
            QStringList info;
            for(int j = 0; j < certififate_info[i].size() - 1; j++){
                info << certififate_info[i][j];
            }
            // 这里一定要先setRowCount,要不然插入的时候没有对应行
            current_table->setRowCount(certififate_info.size() + 100);
            insertTabelItems(row, 0, tableManager.col_num - 1, info, current_table);
        }

    }
    current_table->resizeColumnToContents(2);
    current_table->resizeColumnToContents(3);
    current_table->resizeColumnToContents(4);
    current_table->resizeColumnToContents(5);

    // 新建表格添加行
    if(current_table == ui->tableWidget2 && certififate_info.isEmpty()){
        int ncount = current_table->rowCount();
        current_table->setRowCount(ncount + 100);
    }


    current_table->blockSignals(false);
}

int Widget::modifyTableInfo(int row, int col, QString info, QTableWidget *current_table){
    current_table->blockSignals(true);


    QStringList info_list(info);

    int row_in_database = tableManager.QtableToDatabaseRow(row);

    QString pre_info = xlsx->read(row_in_database, col + 1).toString();
    QStringList pre_info_list(pre_info);

    // 其他几列不作格式要求
    if(info.isEmpty() || col < 4){
        insertTabelItems(row, col, col, info_list, current_table);
        // xlsx的行列从1开始
        tableManager.rewriteItem(xlsx, row_in_database, col + 1, info);
    }
    // 已经做过权限判断了，这里只需要保存数据就行了
    else if(col == 5){
        if(info != tableManager.finished && info != tableManager.ignore && info != tableManager.waiting){
            insertTabelItems(row, col, col, pre_info_list, current_table);
            return -2;
        }
        else{
            insertTabelItems(row, col, col, info_list, current_table);
            // xlsx的行列从1开始
            tableManager.rewriteItem(xlsx, row_in_database, col + 1, info);
            if(pre_info == tableManager.waiting && (info == tableManager.finished || info == tableManager.ignore)){
                QString dateString = xlsx->read(row_in_database, 5).toString();
                QDate date = QDate::fromString(dateString, "yyyyMMdd");
                if(date.isValid()){
                    QDate new_date = date.addYears(1);
                    QString new_dateString = new_date.toString("yyyyMMdd");
                    tableManager.rewriteItem(xlsx, row_in_database, 5, new_dateString);
                }

            }
        }
    }
    else if(col == 4){
        QDate date =  QDate::fromString(info, "yyyyMMdd");
        if(date.isValid()){
            insertTabelItems(row, col, col, info_list, current_table);
            tableManager.rewriteItem(xlsx, row_in_database, col + 1, info);
        }
        else{
            QStringList pre_info(xlsx->read(row_in_database, col + 1).toString());
            insertTabelItems(row, col, col, pre_info, current_table);
            return -1;
        }
    }

    current_table->blockSignals(false);
    return 0;
}

/**********************************************************控件槽函数****************************************************************/

void Widget::openWindow(QString username, QString permission){

    ui->username->setAlignment(Qt::AlignCenter);
    ui->username->setText(username);
    this->username = username;
    this->permission = permission;

    this->setProperty("canMove",true);
    this->initUi();
    this->initMember();

    if(!QFile::exists(config::table_path)){
        server.downloadTable();
    }
    this->initTable();
    this->show();
}


void Widget::closeEvent(QCloseEvent *event)
{
//    myMbox->setIcon("question");
//    myMbox->setBodyText("How to leave?");
//    myMbox->setButtonText("Little","Logout");
//    // 这里当点击Little或logout按钮时，会触发对话框中的函数，将对话框关闭，进而停止exec的阻塞，继续执行
//    myMbox->exec();
//    if(mbox_return == 0) {
//        event->ignore();
//    }else if(mbox_return == 1){
//        event->ignore();
//        littleShow();
//    }
}

void Widget::keyPressEvent(QKeyEvent *keyEvent){
    if(keyEvent->matches(QKeySequence::Copy)){
        dealActionCopyTriggered();
    }
    else if (keyEvent->matches(QKeySequence::Paste)){
        dealActionPasteTriggered();
    }
    else if(keyEvent->matches(QKeySequence::Delete)){
        dealActionDeleteTriggered();
    }
}



//事件过滤
bool Widget::eventFilter(QObject *target, QEvent *event)
{
    // 这行输出千万不能删！！！！删了复制的时候就会崩溃，很玄学，不知道为啥(tmd现在又好了，怀疑是qt自己的bug)
    //qDebug() << "eventFilter";
    //类型转换
    QMouseEvent *e = static_cast<QMouseEvent *>(event);
    //判空
    if (e == nullptr)
    {
        return QWidget::eventFilter(target, event);
    }
    //判断事件类型为鼠标点击
    if (e->type() == QEvent::MouseButtonPress || e->type() == QEvent::MouseButtonRelease)
    {
        //点击空白处取消选中
        QTableWidget *current_table = getCurrentTableWidget();
        current_table->setCurrentItem(NULL);

    }
    return QWidget::eventFilter(target, event);
}



void Widget::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    QTableWidget *current_table = getCurrentTableWidget();
    QModelIndex index = current_table->indexAt(pos);
    qDebug() << index;
    if(index.isValid())
    {
        QMenu *popMenu = new QMenu(current_table);
        QAction *actionCopy = new QAction("复制",popMenu);
        QAction *actionPaste = new QAction("粘贴",popMenu);
        connect(actionCopy, &QAction::triggered, this, &Widget::dealActionCopyTriggered);
        connect(actionPaste, &QAction::triggered, this, &Widget::dealActionPasteTriggered);

        popMenu->addAction(actionCopy);
        popMenu->addAction(actionPaste);
        popMenu->exec(QCursor::pos());

        delete actionCopy;
        delete actionPaste;
        delete popMenu;
    }


}


//复制选中的tableview数据
void Widget::dealActionCopyTriggered()
{
    QTableWidget *current_table = getCurrentTableWidget();
    //获取选中的行列范围
    QList<QTableWidgetItem*> items = current_table->selectedItems();
    if(items.empty())
        return;

    selected_ranges.clear();
    int item_num = items.count();
    //从tableview界面拿数据
    QStringList clipboardTextList;
    for(int i = 0; i < item_num; i++)
    {
        QTableWidgetItem *item = items.at(i);
        clipboardTextList.append(item->text());
        Range range(item->row(), item->column());
        selected_ranges.append(range);
    }
    QString clipboardText = clipboardTextList.join('\n');

    //将数据放入剪贴板
    QApplication::clipboard()->setText(clipboardText);
}

void Widget::dealActionPasteTriggered()
{
    QTableWidget *current_table = getCurrentTableWidget();

    QString clipboardText = QApplication::clipboard()->text();//从剪贴板拿数据
    if(clipboardText.isEmpty())
        return;
    QStringList TextList = clipboardText.split('\n');
    if(TextList.last().isEmpty())
        TextList.removeLast();

    if(TextList.size() != selected_ranges.size())
        return;
    //将数据插入到tableWidget
    QList<QTableWidgetSelectionRange> indexList = current_table->selectedRanges();
    QList<QTableWidgetItem*> items = current_table->selectedItems();
    if(indexList.isEmpty())
        return;


    QMessageBox *no_name;
    int sig = 0;
    // 如果只复制了一个单元格，那么粘贴到所有选中的单元格
    if(selected_ranges.size() == 1){
        QTableWidgetSelectionRange startIndex = indexList.first();
        for(int i = 0; i < items.count(); i++){
            QTableWidgetItem *item = items.at(i);
            sig = modifyTableInfo(item->row(), item->column(), TextList[0], current_table);
        }

    }
    else{
        QTableWidgetSelectionRange startIndex = indexList.first();
        QList<Range> relative_range = selected_ranges.getRelativeRange();
        for(int i = 0; i < TextList.size(); i++)
        {
            QString itemText = TextList[i];
            sig = modifyTableInfo(relative_range[i].row + startIndex.topRow(), relative_range[i].col + startIndex.leftColumn(), itemText, current_table);
        }
    }


    if(sig == -1){
        no_name->warning(this, "警告", "日期格式无效！");
    }
    else if(sig == -2){
        no_name->warning(this, "警告", "状态无效！");
    }
    // 这里有两个save,但是不得不这样，因为refreshTable里要用到数据库表格的数据
    xlsx->save();
    tableManager.refreshTable(xlsx);
}



void Widget::dealActionDeleteTriggered(){
    QTableWidget *current_table = getCurrentTableWidget();
    //获取选中的行列范围
    QList<QTableWidgetItem*> items = current_table->selectedItems();
    if(items.empty())
        return;


    int item_num = items.count();

    QString info = "";
    QStringList info_list(info);
    for(int i = 0; i < item_num; i++)
    {
        int row_in_database = tableManager.QtableToDatabaseRow(items[i]->row());
        // 这两行顺序不能变，要不然item指向的行列都变成-1了
        tableManager.rewriteItem(xlsx, row_in_database, items[i]->column() + 1, info);
        insertTabelItems(items[i]->row(), items[i]->column(), items[i]->column(), info_list, current_table);
        // xlsx的行列从1开始
    }

    xlsx->save();
}

void Widget::changeMboxReturn(int num)
{
    this->mbox_return = num;
}


void Widget::on_btn_logout_clicked()
{
    close();
}

void Widget::on_btn_littleshow_clicked()
{
    showMinimized();
}




void Widget::on_manageSheetButton_clicked()
{
    if(this->permission != userPermissionManager.super){
        QMessageBox *no_name;
        no_name->warning(this, "警告", "无权限！");
    }
    else{
        manageSheet->openWindow(xlsx);
    }

}



void Widget::on_selfButton_clicked()
{
    staffManage->openWindow(this->username, this->permission, true);
}

void Widget::on_ShipTypecomboBox_currentIndexChanged(const QString &arg)
{
    xlsx->selectSheet(arg);
    for(int i = 0; i <= 1; i++){
        QTableWidget *tableWidget = ui->stackedWidget->widget(i)->findChildren<QTableWidget*>()[0];
        QList<QStringList> certififate_info = tableManager.getAllTargetCeritificate(xlsx, tableWidget);
        showTabelInfo(certififate_info, tableWidget);
    }
}



void Widget::on_waitingButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->waitingButton->setStyleSheet("background-color:#AAAAAA; font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->allButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
}

void Widget::on_allButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->allButton->setStyleSheet("background-color:#AAAAAA; font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->waitingButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");

}

void Widget::on_refreshButton_clicked()
{
    server.downloadTable();
    initTable();
    //    QString cur_sheet = xlsx->currentSheet()->sheetName();
    //    xlsx = std::make_shared<QXlsx::Document>(config::table_path);
    //    xlsx->selectSheet(cur_sheet);
    //    for(int i = 0; i <= 1; i++){
    //        QTableWidget *tableWidget = ui->stackedWidget->widget(i)->findChildren<QTableWidget*>()[0];
    //        QList<QStringList> certififate_info = tableManager.getAllTargetCeritificate(xlsx, tableWidget);
    //        showTabelInfo(certififate_info, tableWidget);
    //    }
    QMessageBox *no_name;
    no_name->information(this, "提示", "拉取成功！");
}

void Widget::on_staffManageButton_clicked()
{
    if(this->permission == userPermissionManager.staff){
        QMessageBox *no_name;
        no_name->warning(this, "警告", "无权限！");
    }
    else{
        staffManage->openWindow(this->username, this->permission);
    }

}


void Widget::on_tablewidget_cellChanged(int row,int col){
    QTableWidget *current_table = getCurrentTableWidget();
    // TableWidget对cell change的判断很逆天，访问等操作也算change，所以要先把信号阻塞
    current_table->blockSignals(true);
    QMessageBox *no_name;
    QString info = current_table->item(row, col)->text();
    int sig = modifyTableInfo(row, col, info, current_table);
    if(sig == -1){
        no_name->warning(this, "警告", "日期格式无效！");
    }
    else if(sig == -2){
        no_name->warning(this, "警告", "状态无效！");
    }
    // 这里有两个save,但是不得不这样，因为refreshTable里要用到数据库表格的数据
    xlsx->save();
    tableManager.refreshTable(xlsx);
    current_table->blockSignals(false);
}

void Widget::on_managesheet_addsheet(QString sheetname){
    ui->ShipTypecomboBox->clear();
    ui->ShipTypecomboBox->addItems(tableManager.getShipType(xlsx));
    xlsx->selectSheet(sheetname);
    ui->ShipTypecomboBox->setCurrentText(xlsx->currentSheet()->sheetName());
}


void Widget::on_managesheet_deletesheet(){
    // 下面重置了ComboBox，xlsx->currentSheet重置了
    QString cur_name = xlsx->currentSheet()->sheetName();
    ui->ShipTypecomboBox->clear();
    ui->ShipTypecomboBox->addItems(tableManager.getShipType(xlsx));

    xlsx->selectSheet(cur_name);
    ui->ShipTypecomboBox->setCurrentText(xlsx->currentSheet()->sheetName());
}


void Widget::on_staffManage_nameChange(QString name){
    this->username = name;
    ui->username->setText(username);
}



void Widget::on_uploadButton_clicked()
{
    QMessageBox *no_name;
    if(this->permission != userPermissionManager.super){
        no_name->warning(this, "警告", "无权限！");
    }
    else{
        tableManager.refreshTable(xlsx);
        server.uploadFileToNextcloud(config::table_path, config::nextcloud_url, config::nextcloud_username, config::nextcloud_password);
        no_name->information(this, "提示", "上传成功！");
    }
}

void Widget::on_pullLocalButton_clicked()
{
        QString cur_sheet = xlsx->currentSheet()->sheetName();
        xlsx = std::make_shared<QXlsx::Document>(config::table_path);
        xlsx->selectSheet(cur_sheet);
        for(int i = 0; i <= 1; i++){
            QTableWidget *tableWidget = ui->stackedWidget->widget(i)->findChildren<QTableWidget*>()[0];
            QList<QStringList> certififate_info = tableManager.getAllTargetCeritificate(xlsx, tableWidget);
            showTabelInfo(certififate_info, tableWidget);
        }
}
