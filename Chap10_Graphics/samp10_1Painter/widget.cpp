#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPaintEvent>

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

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);        // 图片抗锯齿
    painter.setRenderHint(QPainter::TextAntialiasing);    // 文字抗锯齿

    int w=this->width();
    int h=this->height();

    QRect rect(w/4,h/4,w/2,h/2);
    QPen pen;
    pen.setWidth(3);
    pen.setColor(Qt::red);
    pen.setStyle(Qt::DashLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);

    QBrush brush;
    brush.setColor(Qt::yellow);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);

    painter.drawRect(rect);

    event->accept();
}
