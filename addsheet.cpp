#include "addsheet.h"
#include "ui_addsheet.h"

AddSheet::AddSheet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSheet)
{
    ui->setupUi(this);
}

AddSheet::~AddSheet()
{
    delete ui;
}
