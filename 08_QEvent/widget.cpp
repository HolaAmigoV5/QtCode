#include "widget.h"
#include "ui_widget.h"
#include <QTimerEvent>
#include <QTimer>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 启动定时器
    id1= startTimer(1000);
    startTimer(2000);

    // 定时器第二种实现方式
    QTimer *timer=new QTimer(this);
    timer->start(500);
    connect(timer, &QTimer::timeout,[=](){
        static int num=0;
        ui->label_3->setText(QString::number(num++));
    });

    bool flag;
    connect(ui->btn, &QPushButton::clicked, [=]()mutable{
       flag=!flag;
        if(flag){
           ui->btn->setText("启动");
           timer->stop();
        }
       else{
            ui->btn->setText("暂停");
           timer->start(500);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *ev){
    if(ev->timerId()==id1){
        static int num=1;
        ui->label_2->setText(QString::number(num++));
    }
    else{
        static int num2=1;
        ui->lcdNumber->display(QString::number(num2++));
    }

}


