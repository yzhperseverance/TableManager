#ifndef TABEL_H
#define TABEL_H

#include "xlsxdocument.h"
#include <QDate>
#include <QList>
#include <memory>
#include <QTableWidget>
//#include <iostream>
using namespace std;
class TableManager
{

public:
    TableManager(){}

    //inline QXlsx::Document* ReadXlsx(QString path){ return &QXlsx::Document(path);}
    // 获取船的种类
    QStringList getShipType(shared_ptr<QXlsx::Document> &xlsx);

    QList<QStringList> getAllTargetCeritificate(shared_ptr<QXlsx::Document> &xlsx, QTableWidget *current_widget);
    // 获取指定证书在表格中的行数
    int QtableToDatabaseRow(int row);

    int DatabaseToQtableRow(int row);
    // 刷新证书的状态
    void refreshTable(shared_ptr<QXlsx::Document> &xlsx);
    // （覆盖）写入数据
    void rewriteItem(shared_ptr<QXlsx::Document> &xlsx, int row, int col, QVariant item);
    // QXlsx库对表格行数的计算有问题，表格某一行数据为空，但仍会被计算到行数里，因为没有把他写成blank，所以自己重写一个获取表格行数的函数
    // 好像其实没问题？o.O
    int getRowNum(shared_ptr<QXlsx::Document> &xlsx);
    //void addSheet(shared_ptr<QXlsx::Document> &xlsx)
    // 如果只有一个"已检"状态，则在三个月内自动更新为未检时，会分不清是去年检查完保持的未检状态还是今年已经检查完了，所以要分成两个状态
    const QString finished = "已检"; // 表示今年的检测任务做完了，当离日期前后三个月时是这个状态
    const QString waiting = "未检";
    const QString ignore = "忽略";


    const int col_num = 6; // 表格列数
};



#endif // TABEL_H
