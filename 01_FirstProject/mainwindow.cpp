#include "mainwindow.h"
#include "mypushbutton.h"
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QPushButton *btn1=new QPushButton;
    btn1->setParent(this);
    btn1->setText("第一个按钮");
    btn1->adjustSize();
    btn1->show();



    QPushButton *btn2=new QPushButton("第二个按钮", this);
    btn2->move(100,100);

    resize(800,600);

    setWindowTitle("窗口");

    MyPushButton *btn3=new MyPushButton();
    btn3->move(200,200);
    btn3->setText("我的自定义按钮");
    btn3->setParent(this);
    btn3->adjustSize();

    connect(btn3,&MyPushButton::clicked,this, &MainWindow::close);
}

MainWindow::~MainWindow()
{
    qDebug()<<"MainWindow析构了";
}

