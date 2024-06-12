#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QMouseEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    grabKeyboard();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        QPoint pt=event->pos();// 相对widget的位置
        QPointF relaPt=event->position();
        QPointF winPt=event->scenePosition();
        QPointF globPt=event->globalPosition();   // 相对屏幕的绝对位置
        QString str=QString::asprintf("pos()=(%d, %d)",pt.x(),pt.y());
        str+=QString::asprintf("\nposition()=(%.0f, %.0f)",relaPt.x(),relaPt.y());
        str+=QString::asprintf("\nscenePosition()=(%.0f, %.0f)",winPt.x(),winPt.y());
        str+=QString::asprintf("\nglobalPos()=(%.0f, %.0f)",globPt.x(),globPt.y());

        ui->labMove->setText(str);
        ui->labMove->adjustSize();
        ui->labMove->move(event->pos());
    }

    QWidget::mousePressEvent(event);
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    QPoint pt=ui->btnMove->pos();
    if(event->key()==Qt::Key_A || event->key()==Qt::Key_Left)
        ui->btnMove->move(pt.x()-20,pt.y());
    if(event->key()==Qt::Key_D || event->key()==Qt::Key_Right)
        ui->btnMove->move(pt.x()+20,pt.y());
    if(event->key()==Qt::Key_W || event->key()==Qt::Key_Up)
        ui->btnMove->move(pt.x(),pt.y()-20);
    if(event->key()==Qt::Key_S || event->key()==Qt::Key_Down)
        ui->btnMove->move(pt.x(),pt.y()+20);

    QWidget::keyPressEvent(event);
}

void Widget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(0,0,width(),height(),QPixmap(":/pics/images/background.jpg"));
}

// 关闭
#include <QMessageBox>
void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton res=QMessageBox::question(this,"提示","是否需要关闭?");

    if(res==QMessageBox::Yes)
        event->accept();
    else
        event->ignore();
}
