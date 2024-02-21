#ifndef MANAGESHEET_H
#define MANAGESHEET_H

#include <QWidget>
#include <memory>
#include <QString>
#include <QScreen>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include "xlsxdocument.h"

namespace Ui {
class ManageSheet;
}

class ManageSheet : public QWidget
{
    Q_OBJECT

public:
    explicit ManageSheet(QWidget *parent = 0);
    ~ManageSheet();

    void initTable();

    void openWindow(std::shared_ptr<QXlsx::Document> &xlsx);

    void showSheetInfo();

    void on_tablewidget_cellChanged(int row,int col);

    int findSheet(QString name);
private slots:
    void on_addSheetButton_clicked();

    void on_deleteSheetButton_clicked();

signals:
    void addsheetSignal(QString sheetname);

    void deletesheetSignal();

private:
    Ui::ManageSheet *ui;
    std::shared_ptr<QXlsx::Document> xlsx;
    QStringList sheet_names;

};

#endif // MANAGESHEET_H
