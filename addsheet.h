#ifndef ADDSHEET_H
#define ADDSHEET_H

#include <QWidget>

namespace Ui {
class AddSheet;
}

class AddSheet : public QWidget
{
    Q_OBJECT

public:
    explicit AddSheet(QWidget *parent = 0);
    ~AddSheet();

private:
    Ui::AddSheet *ui;
};

#endif // ADDSHEET_H
