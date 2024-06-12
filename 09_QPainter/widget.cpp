#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->btn, QPushButton::clicked, [=](){
        posX+=20;
        update();
    });

    QTimer *timer=new QTimer(this);
    timer->start(500);
    connect(timer, &QTimer::timeout,[=](){
        posX+=20;
        update();
    });
}

Widget::~Widget()
{
    delete ui;
}


void Widget::paintEvent(QPaintEvent *pe){
//    QPainter painter(this);

//    QPen pen(QColor(255,0,0));
//    pen.setWidth(5);
//    pen.setStyle(Qt::DotLine);
//    painter.setPen(pen);


//    // 设置画刷
//    QBrush brush(Qt::cyan);
//    painter.setBrush(brush);

//    painter.drawLine(0,0,100,100);
//    painter.drawEllipse(100,100,100,100);
//    painter.drawRect(QRect(20,20,50,50));
//    painter.drawText(QRect(10,200,150,50), "好好学习，天天向上。");


    // -----------------------------高级设置--------------------------
    QPainter painter(this);

//    painter.drawEllipse(QPoint(100,50),50,50);
//    // 设置抗锯齿
//    painter.setRenderHint(QPainter::Antialiasing);
//    painter.drawEllipse(QPoint(200,50),50,50);

    // 画矩形
//    painter.drawRect(QRect(20,20,50,50));
//    painter.translate(100,0);
//    painter.drawRect(QRect(20,20,50,50));

    if(posX+200>=this->width())
        posX=0;

    // 画资源图片
    painter.drawPixmap(posX,20,QPixmap(":/Images/jt.png"));

}

