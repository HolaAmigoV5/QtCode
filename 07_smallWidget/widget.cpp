#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent): QWidget(parent) , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->btnGetValue, &QPushButton::clicked,this,[=](){
        qDebug()<<ui->widget->GetCurrentValue();
    });
    connect(ui->btnSetValue, &QPushButton::clicked,this,[=](){
        ui->widget->SetHalfValue(50);});
}

Widget::~Widget()
{
    delete ui;
}

