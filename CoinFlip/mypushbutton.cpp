#include "mypushbutton.h"
#include <QPixmap>
#include <QMessageBox>
#include <QPropertyAnimation>


MyPushButton::MyPushButton(QString normalImg, QString pressImg):
    normalImgPath(normalImg), pressedImgPath(pressImg)
{
    SetImg(normalImg);
}

void MyPushButton::SetImg(QString imgPath){
    QPixmap pixmap;
    if(!pixmap.load(imgPath)){
        QMessageBox::critical(this,"错误", "图片加载错误");
        return;
    }
    setFixedSize(pixmap.width(), pixmap.height());  // 设置图片固定大小
    setStyleSheet("QPushButton{border:0px};");  // 设置不规则图片样式
    setIcon(pixmap);    // 设置图标
    setIconSize(QSize(pixmap.width(), pixmap.height())); // 设置图标大小
}

// 动画特效。向下跳跃
void MyPushButton::zoom1(){
    QPropertyAnimation *animation1=new QPropertyAnimation(this,"geometry");
    animation1->setDuration(200);

    // 创建起始位置
    animation1->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation1->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    // 设置缓和曲线，QEasingCurve::OutBounce 弹跳效果
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

// 向上跳跃
void MyPushButton::zoom2(){
    QPropertyAnimation *animation1=new QPropertyAnimation(this,"geometry");
    animation1->setDuration(200);

    // 创建起始位置
    animation1->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    animation1->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    // 设置缓和曲线，QEasingCurve::OutBounce 弹跳效果
    animation1->setEasingCurve(QEasingCurve::OutBounce);
    animation1->start();
}

void MyPushButton::mousePressEvent(QMouseEvent *e){
    if(this->pressedImgPath!=""){
        SetImg(pressedImgPath);
    }

    // 让父类执行其他内容
    return QPushButton::mousePressEvent(e);
}
void MyPushButton::mouseReleaseEvent(QMouseEvent *e){
    if(this->pressedImgPath!=""){
        SetImg(normalImgPath);
    }
    return QPushButton::mouseReleaseEvent(e);
}
