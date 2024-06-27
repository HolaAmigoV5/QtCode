#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QPaintEvent>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    setPalette(QPalette(Qt::white));
    setAutoFillBackground(true);
    resize(300,300);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    int w=width();
    int h=height();
    int size=qMin(w,h);

    QRect rect((w-size)/2,(h-size)/2,size,size);
    painter.drawRect(rect);

    painter.setViewport(rect);
    painter.setWindow(-100,-100,200,200);

    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);

    QLinearGradient linearGrid(0,0,100,0);
    linearGrid.setColorAt(0,Qt::yellow);
    linearGrid.setColorAt(1,Qt::green);
    painter.setBrush(linearGrid);

    //painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
    //painter.setCompositionMode(QPainter::CompositionMode_Difference);
    painter.setCompositionMode(QPainter::RasterOp_NotSourceXorDestination);

    for(int i=0; i<36; i++){
        painter.drawEllipse(QPoint(50,0),50,50);
        painter.rotate(10);
    }

    event->accept();
}
