#include "tdialoglocate.h"
#include "ui_tdialoglocate.h"

TDialogLocate::TDialogLocate(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDialogLocate)
{
    ui->setupUi(this);
}

TDialogLocate::~TDialogLocate()
{
    delete ui;
}

void TDialogLocate::setSpinRange(int rowCount, int colCount)
{
    ui->spinBoxRow->setMaximum(rowCount-1);
    ui->spinBoxColumn->setMaximum(colCount-1);
}

void TDialogLocate::on_btnSetText_clicked()
{
    QString text=ui->edtCaption->text();
    int row =ui->spinBoxRow->value();
    int column=ui->spinBoxColumn->value();

    if(ui->chkBoxRow->isChecked()){
        ui->spinBoxRow->setValue(row+1);
    }
    if(ui->chkBoxColumn->isChecked()){
        ui->spinBoxColumn->setValue(column+1);
    }

    emit changeCellText(row,column,text);
}

void TDialogLocate::setSpinValue(int rowNo, int ColNo)
{
    ui->spinBoxColumn->setValue(ColNo);
    ui->spinBoxRow->setValue(rowNo);
}

#include <QCloseEvent>
void TDialogLocate::closeEvent(QCloseEvent *event)
{
    event->accept();
    emit changeActionEnable(true);
}

void TDialogLocate::showEvent(QShowEvent *event)
{
    event->accept();
    emit changeActionEnable(false);
}

