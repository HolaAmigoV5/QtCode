#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_horizontalSlider_valueChanged(int value)
{
    ui->tBattery->setPowerLevel(value);
    ui->labInfo->setText(QString::asprintf("当前电量：%d %%",value));
}

