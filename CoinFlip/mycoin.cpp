#include "mycoin.h"
#include <QDebug>


MyCoin::MyCoin(QString butImg){
    SetImg(butImg);
    timer1=new QTimer(this);
    timer2=new QTimer(this);

    // 监听正面翻转的信号槽
    connect(timer1, &QTimer::timeout, this, [=](){
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);
        SetImg(str);
        if(this->min>this->max){
            this->min=1;
            timer1->stop();
            isAnimation=false;
        }
    });

    connect(timer2, &QTimer::timeout, this, [=](){
        QString str=QString(":/res/Coin000%1.png").arg(this->max--);
        SetImg(str);
        if(this->max<this->min){
            this->max=8;
            timer2->stop();
            isAnimation=false;
        }
    });
}

void MyCoin::changeFlag(){
    if(this->flag){
        // 如果是正面
        timer1->start(30);
        this->flag=false;
    }
    else{
        timer2->start(30);
        this->flag=true;
    }
    isAnimation=true;
}

void MyCoin::SetImg(QString imgPath){
    QPixmap pixmap;
    if(pixmap.load(imgPath)){
        setFixedSize(pixmap.width(), pixmap.height());  // 设置图片固定大小
        setStyleSheet("QPushButton{border:0px};");  // 设置不规则图片样式
        setIcon(pixmap);    // 设置图标
        setIconSize(QSize(pixmap.width(), pixmap.height())); // 设置图标大小
    }
}

void MyCoin::mousePressEvent(QMouseEvent *e){
    if(isAnimation ||isWin)
        return;

    QPushButton::mousePressEvent(e);
}
