#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
//最小化相关
#include <QSystemTrayIcon>
#include <QCloseEvent>
#include <QMenu>
#include <QAction>
#include <QEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include <QPixmap>
#include <QThread>
#include <QKeyEvent>
#include <QString>
#include <QVariant>
#include <QWidget>
#include <QDebug>
#include <QScreen>
#include <QLineEdit>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QTableWidget>
#include <QClipboard>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QList>
#include <memory>
#include "table.h"
#include "config.h"
#include "database.h"
#include "ui_widget.h"
#include "staffmanage.h"
#include "permission.h"
#include "managesheet.h"
//自定义弹窗
#include "mymessagebox.h"

namespace Ui {
class Widget;
}


struct Range{
    int row, col;
    Range(){}
    Range(int r, int c):row(r), col(c){}
};


// 用来记录选中单元格的数据结构
class myRange{
public:


    inline QList<Range> get(){
        return ranges;
    }
    // 相对位置信息
    QList<Range> getRelativeRange(){
        QList<Range> relative_range;
        if(ranges.empty()) return relative_range;
        for(int i = 0; i < ranges.size(); i++){
            int row = ranges[i].row - ranges[0].row;
            int col = ranges[i].col - ranges[0].col;
            Range range(row, col);
            relative_range.append(range);
        }
        return relative_range;
    }

    inline void append(Range range){
        ranges.append(range);
    }

    inline int size(){
        return ranges.size();
    }

    inline void clear(){
        ranges.clear();
    }


private:
    QList<Range> ranges;

};




class Widget : public QWidget
{
    Q_OBJECT

public:




    explicit Widget(QWidget *parent = 0);

    void openWindow(QString username, QString permission);

    void keyPressEvent(QKeyEvent *keyEvent);

    ~Widget();

private slots:

    void closeEvent(QCloseEvent *event);

    //事件过滤
    bool eventFilter(QObject *obj, QEvent *e);

    void changeMboxReturn(int num);

    void on_btn_logout_clicked();

    void on_btn_littleshow_clicked();

    void on_selfButton_clicked();


    void on_manageSheetButton_clicked();

    void on_ShipTypecomboBox_currentIndexChanged(const QString &arg1);

    void on_waitingButton_clicked();

    void on_allButton_clicked();

    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void on_staffManageButton_clicked();

    void on_tablewidget_cellChanged(int row,int col);

    void on_refreshButton_clicked();

    void on_managesheet_addsheet(QString sheetname);

    void on_managesheet_deletesheet();

    void on_staffManage_nameChange(QString name);



    void on_uploadButton_clicked();

    void on_pullLocalButton_clicked();

private:
    Ui::Widget *ui;

    MyMessageBox *myMbox;
    //自定义窗口通信参数
    int mbox_return;

    //最小化到托盘
    QSystemTrayIcon *trayIcon;

    QMenu *trayIconMenu;
    //窗口管理动作
    QAction *returnNormal;
    QAction *quitAction;
    // 表格管理
    TableManager tableManager;

    StaffManage *staffManage;

    ManageSheet *manageSheet;

    QString username, permission;

    CommunicateWithServer server;

    std::shared_ptr<QXlsx::Document> xlsx;

    UserPermissionManager userPermissionManager;

    myRange selected_ranges;

    // Ui界面初始化函数
    void initUi();
    // 成员变量初始化函数
    void initMember();
    // 表格初始化
    void initTable();
    // 最小化显示函数
    void littleShow();
    // 显示表格信息
    void showTabelInfo(QList<QStringList> certififate_info, QTableWidget *current_table);
    // 插入item
    void insertTabelItems(int row, int start_col, int end_col, QStringList item, QTableWidget *current_table);
    // 删除item
    void deleteItem(int row, int col);
    // 获取当前表格
    QTableWidget* getCurrentTableWidget() const { return ui->stackedWidget->currentWidget()->findChildren<QTableWidget*>()[0];}
    // 复制按键处理
    void dealActionCopyTriggered();
    // 粘贴按键处理
    void dealActionPasteTriggered();
    // 删除按键处理
    void dealActionDeleteTriggered();
    // 带文本检测的表格数据插入,0->成功插入，-1->日期格式有问题，-2->状态格式有问题
    int modifyTableInfo(int row, int col, QString info, QTableWidget *current_table);

};

#endif // WIDGET_H
