#include "managesheet.h"
#include "ui_managesheet.h"

ManageSheet::ManageSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageSheet)
{
    ui->setupUi(this);
    connect(ui->tableWidget, &QTableWidget::cellChanged, this, &ManageSheet::on_tablewidget_cellChanged);
    ui->addSheetButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");
    ui->deleteSheetButton->setStyleSheet("font-family:'Microsoft YaHei UI'; font-size: 12pt");

    // 获取当前屏幕分辨率
    const QSize screenSize = qApp->primaryScreen()->size();
    const int screenWidth = screenSize.width();
    const int screenHeight = screenSize.height();
    // 设置缩放比例
    qreal scaleX = static_cast<qreal>(screenWidth) / 1920.0;  // 根据自己期望的基准分辨率调整，这里以1920x1080为例
    qreal scaleY = static_cast<qreal>(screenHeight) / 1080.0;
    this->resize(620 * scaleX, 533 * scaleY);
}

ManageSheet::~ManageSheet()
{
    delete ui;
}



void ManageSheet::initTable(){
    this->setWindowTitle("管理表格");
    QStringList header;
    ui->tableWidget->setColumnCount(1);
    header << "表格名称";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    // 设置列宽不能被手动拉伸，要放在设置列宽前
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    // 设置表头字体
    ui->tableWidget->horizontalHeader()->setStyleSheet("font-family:'Arial'; font-size: 10pt; font-weight: bold;");
    // 自适应列宽
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置表头中间对齐
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    // 设置奇偶行颜色区分
    ui->tableWidget->setAlternatingRowColors(true);
    // 设置窗口置顶
    ui->tableWidget->raise();
    ui->tableWidget->activateWindow();
}

void ManageSheet::openWindow(std::shared_ptr<QXlsx::Document> &xlsx){
    this->setWindowTitle("管理表格");
    this->xlsx = xlsx;
    this->sheet_names = xlsx->sheetNames();
    initTable();
    showSheetInfo();
    this->show();
}


void ManageSheet::showSheetInfo(){
    ui->tableWidget->blockSignals(true);
    ui->tableWidget->setRowCount(0);
    for(int i = 0; i < sheet_names.size(); i++){
        ui->tableWidget->insertRow(i);
        QTableWidgetItem *item = new QTableWidgetItem(sheet_names[i]);
        item->setFont(QFont("微软雅黑", 10));
        item->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, item);
    }
    ui->tableWidget->blockSignals(false);
}


void ManageSheet::on_tablewidget_cellChanged(int row,int col){
    ui->tableWidget->blockSignals(true);

    QString old_name = sheet_names[row];
    QString new_name = ui->tableWidget->item(row, col)->text();
    if(findSheet(new_name) != -1){
        QMessageBox *no_name;
        no_name->warning(this, "警告", "表格名重复！");
    }
    else{
        xlsx->renameSheet(old_name, new_name);
        this->sheet_names = xlsx->sheetNames();
        emit addsheetSignal(new_name);
        xlsx->save();
    }

    ui->tableWidget->blockSignals(false);


}


int ManageSheet::findSheet(QString name){

    for(int i = 0; i < sheet_names.size(); i++){
        if(name == sheet_names[i]){
            return i;
        }
    }

    return -1;

}

void ManageSheet::on_addSheetButton_clicked()
{
    // 防止出bug，保险起见
    this->sheet_names = xlsx->sheetNames();

    QString sheetname;
    for(int i = 1; this->sheet_names.size() == xlsx->sheetNames().size(); i++){
        sheetname = "sheet" + QString("%1").arg(sheet_names.size() + i);;
        xlsx->addSheet(sheetname, QXlsx::AbstractSheet::ST_WorkSheet);
    }
    qDebug() << xlsx->sheetNames();
    xlsx->save();
    this->sheet_names = xlsx->sheetNames();
    showSheetInfo();
    emit addsheetSignal(sheetname);
}

void ManageSheet::on_deleteSheetButton_clicked()
{
    QMessageBox *no_name;
    QList<QTableWidgetSelectionRange> indexList = ui->tableWidget->selectedRanges();
    if(indexList.isEmpty()){
        no_name->warning(this, "警告", "请选中表格名称！");
        return;
    }
    if(xlsx->sheetNames().size() == 1){
        no_name->warning(this, "警告", "删除失败！表格数量至少为1！");
        return;
    }
    int startRow = indexList.first().topRow();
    int endRow = indexList.last().bottomRow();

    for(int i = startRow;i <= endRow;i++)
    {
        QString name = ui->tableWidget->item(i, 0)->text();
        xlsx->deleteSheet(name);

    }
    xlsx->save();
    this->sheet_names = xlsx->sheetNames();

    showSheetInfo();
    no_name->information(this, "提示", "删除成功！");
    emit deletesheetSignal();
}
