#include "mylabel.h"
#include <QDebug>
#include <QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{

}

//void MyLabel::enterEvent(QEvent *event){
//    qDebug()<<"鼠标进入了";
//}

//void MyLabel::leaveEvent(QEvent *event){
//    qDebug()<<"鼠标离开了";
//}

void MyLabel::mousePressEvent(QMouseEvent *ev){
    if(ev->button()==Qt::LeftButton)
        qDebug()<<QString("鼠标按下了。 x=%1, y=%2, globalx=%3, globaly=%4").arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev){
    if(ev->button()==Qt::LeftButton)
        qDebug()<<QString("鼠标释放了。 x=%1, y=%2, globalx=%3, globaly=%4").arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev){
    if(ev->buttons()&Qt::LeftButton)
        qDebug()<<QString("鼠标移动了。 x=%1, y=%2, globalx=%3, globaly=%4").arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());
}

bool MyLabel::event(QEvent *e){
    if(e->type()==QEvent::MouseButtonPress){
        QMouseEvent* ev=static_cast<QMouseEvent *>(e);
        qDebug()<<QString("Event函数中，鼠标按下了：x=%1, y=%2, globalx=%3, globaly=%4").arg(ev->x()).arg(ev->y()).arg(ev->globalX()).arg(ev->globalY());

        return true;    // 鼠标按下事件自己处理，不向下分发
    }

    return QLabel::event(e);    // 其他事件，交给父类处理， 默认处理。
}


