#include "tdialogsize.h"
#include "ui_tdialogsize.h"

TDialogSize::TDialogSize(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDialogSize)
{
    ui->setupUi(this);
}

TDialogSize::~TDialogSize()
{
    delete ui;
}

void TDialogSize::setRowColumn(int row, int column)
{
    ui->spinBoxColumn->setValue(column);
    ui->spinBoxRow->setValue(row);
}

int TDialogSize::rowCount()
{
    return ui->spinBoxRow->value();
}

int TDialogSize::columnCount()
{
    return ui->spinBoxColumn->value();
}
