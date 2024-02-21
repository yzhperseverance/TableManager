#include "table.h"
#include <QDebug>

QStringList TableManager::getShipType(shared_ptr<QXlsx::Document> &xlsx){
    return xlsx->sheetNames();
}

void TableManager::refreshTable(shared_ptr<QXlsx::Document> &xlsx){

//    qDebug() << "原行数:" << xlsx->dimension().rowCount();
//    int rowLen = getRowNum(xlsx);           // 获取最大行数
//    qDebug() << "现行数:" << rowLen;

    int rowTop = xlsx->dimension().topLeft().row();
    int rowBottom = xlsx->dimension().bottomLeft().row();

    // 遍历证书到期时间那一列
    for(int i = rowTop; i <= rowBottom; i++){
        QString dateString = xlsx->read(i, 5).toString();
        QString stateString = xlsx->read(i, 6).toString();

        // 这里得保证日期合法，要不然后面和今天的日期比较的时候会出问题
        if(dateString.isNull() || stateString == "状态" || stateString.isNull()) continue;
        QDate date = QDate::fromString(dateString, "yyyyMMdd");
        QDate today = QDate::currentDate();
        // 指定日期前三个月开始提示
        QDate remind_day_front = date.addMonths(-3);
        QDate remind_day_behind = date.addMonths(3);

        // 只有月份的1号提示，比如5月31号到期，那就从2月1号开始提示，按月算不是按天算
        QString date2string_front = remind_day_front.toString("yyyyMMdd").left(6) + "01";
        QString date2string_behind = remind_day_behind.toString("yyyyMMdd").left(6) + "01";

        remind_day_front =  QDate::fromString(date2string_front, "yyyyMMdd");
        remind_day_behind =  QDate::fromString(date2string_behind, "yyyyMMdd");

        if(today >= remind_day_front){
            rewriteItem(xlsx, i, 6, waiting);
        }
//        // 原来是未检的在没有人工输入的情况下还是未检，不用管
//        if(stateString == finished_maintain || stateString == ignore_maintain){
//            if(today >= remind_day_front){
//                rewriteItem(xlsx, i, 6, waiting);
//            }
//        }
//        else if(stateString == finished){
//            if(today > remind_day_behind){
//                rewriteItem(xlsx, i, 6, finished_maintain);
//                QDate new_date = date.addYears(1);
//                rewriteItem(xlsx, i, 5, new_date.toString("yyyyMMdd"));

//            }
//        }
//        else if(stateString == ignore){
//            if(today > remind_day_behind){
//                rewriteItem(xlsx, i, 6, ignore_maintain);
//                QDate new_date = date.addYears(1);
//                rewriteItem(xlsx, i, 5, new_date.toString("yyyyMMdd"));
//            }
//        }

    }
    xlsx->save();
    
    
}

QList<QStringList> TableManager::getAllTargetCeritificate(shared_ptr<QXlsx::Document> &xlsx, QTableWidget *current_widget){
    // 先更新一遍表格
    refreshTable(xlsx);
    int rowTop = xlsx->dimension().topLeft().row();
    int rowBottom = xlsx->dimension().bottomLeft().row();
    QList<QStringList> res;
    // 遍历证书到期时间那一列
    for(int i = rowTop; i <= rowBottom; i++){
        // 显示未检证书
        if(current_widget->objectName() == "tableWidget1"){
            QString state = xlsx->read(i, 6).toString();
            if(state == waiting){
                QStringList strlist;
                for(int j = 1; j <= 6; j++){
                    QString data = xlsx->read(i, j).toString();
                    strlist.append(data);
                }
                res.append(strlist);
            }
        }
        // 显示全部证书
        else if(current_widget->objectName() == "tableWidget2"){
            QVariant state = xlsx->read(i, 6);
            if(DatabaseToQtableRow(i) < 0) continue;
            //if(state != waiting && state != finished && state != ignore) continue;
            QStringList strlist;
            for(int j = 1; j <= 6; j++){
                QString data = xlsx->read(i, j).toString();
                strlist.append(data);
            }
            // 最后一项储存其所在行
            // -3是因为表头占了3行
            strlist.append(QString("%1").arg(DatabaseToQtableRow(i)));
            res.append(strlist);
        }


    }
    return res;

}


int TableManager::QtableToDatabaseRow(int row){
    /*
    if(target_name.isEmpty())
        return -1;
    int rowTop = xlsx->dimension().topLeft().row();
    int rowBottom = xlsx->dimension().bottomLeft().row();
    // 遍历证书到期时间那一列
    for(int i = rowTop; i <= rowBottom; i++){
        // 显示未检证书
        QString name = xlsx->read(i, 1).toString();
        if(name == target_name){
            return i;
         }
     }
    */
    // 前端页面表格行数与数据库表格行数差4（表头3行+索引从1开始）
    return row + 4;

}

int TableManager::DatabaseToQtableRow(int row){
    return row - 4;
}

int TableManager::getRowNum(shared_ptr<QXlsx::Document> &xlsx){
    int rowTop = xlsx->dimension().topLeft().row();
    int rowBottom = xlsx->dimension().bottomLeft().row();
    int columnLen = xlsx->dimension().columnCount();
    qDebug() << rowTop << " " << rowBottom;

    bool top_full = false, bottom_full = false;
    for(int j = 1; j <= columnLen; j++){
        // 如果QXlsx计算的最底行和最顶行有数据，那说明他算对了
        if(!xlsx->read(rowTop, j).toString().isEmpty()){
            top_full = true;
        }
        if(!xlsx->read(rowBottom, j).toString().isEmpty()){
            bottom_full = true;
        }
    }
    if(top_full && bottom_full)
        return xlsx->dimension().rowCount();

    if(!top_full){
        bool sig = false;
        for(int i = 1; i <= rowBottom; i++){
            for(int j = 1; j <= columnLen; j++){
                if(!xlsx->read(i, j).toString().isEmpty()){
                    xlsx->dimension().setFirstRow(i);
                    sig = true;
                    break;
                }
            }
            if(sig) break;
            if(i == rowBottom)
                xlsx->dimension().setLastRow(rowTop);
        }
    }

    if(!bottom_full){
        bool sig = false;
        // 此时rowTop行一定是有数据的
        for(int i = rowBottom; i >= rowTop; i--){
            for(int j = 1; j <= columnLen; j++){
                if(!xlsx->read(i, j).toString().isEmpty()){
                    xlsx->dimension().setLastRow(i);
                    sig = true;
                    break;
                }
            }
            if(sig) break;

        }
    }


    return xlsx->dimension().rowCount();


}

void TableManager::rewriteItem(shared_ptr<QXlsx::Document> &xlsx, int row, int col, QVariant item){
    // 删除的时候不能打开表格，否则会失败
    xlsx->currentWorksheet()->writeBlank(row, col);
    xlsx->write(row, col, item);
}


