#include "widget.h"
#include "ui_widget.h"
#include <QTableWidgetItem>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // TableWidget 控件
    ui->tableWidget->setColumnCount(3);

    // 设置表头
    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"姓名"<<"性别"<<"年龄");


    // 设置行数
    ui->tableWidget->setRowCount(5);
    //填充数据
    //QTableWidgetItem *item1=new QTableWidgetItem("张三");

    QStringList nameList;
    nameList<<"张飞"<<"赵云"<<"关羽"<<"周瑜"<<"刘备";

    for (int i=0;i<5 ;i++ ) {
        int col=0;
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(nameList[i]));
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(i%2==0?"男":"女"));
        ui->tableWidget->setItem(i,col++,new QTableWidgetItem(QString::number(22+i)));
    }

}

Widget::~Widget()
{
    delete ui;
}

