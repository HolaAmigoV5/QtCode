#ifndef TDIALOGSIZE_H
#define TDIALOGSIZE_H

#include <QDialog>

namespace Ui {
class TDialogSize;
}

class TDialogSize : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogSize(QWidget *parent = nullptr);
    ~TDialogSize();

public:
    void setRowColumn(int row, int column);
    int rowCount();
    int columnCount();

private:
    Ui::TDialogSize *ui;
};

#endif // TDIALOGSIZE_H
