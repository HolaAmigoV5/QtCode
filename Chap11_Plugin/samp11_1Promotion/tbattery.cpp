#include "tbattery.h"
#include <QPainter>
#include <QBrush>
#include <QPaintEvent>

TBattery::TBattery(QWidget *parent)
    : QWidget{parent}
{}

void TBattery::setPowerLevel(int pow)
{
    m_powerLevel=pow;
    emit powerLevelChanged(pow);
    repaint();
}

int TBattery::powerLevel()
{
    return m_powerLevel;
}

void TBattery::setWarnLevel(int warn)
{
    m_warnLevel=warn;
    repaint();
}

int TBattery::warnLevel()
{
    return m_warnLevel;
}

QSize TBattery::sizeHint()
{
    int H=this->height();
    int W=H*12/5;
    QSize size(W,H);
    return size;
}

void TBattery::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QRect rect(0,0,width(),height());
    painter.setViewport(rect);
    painter.setWindow(0,0,120,50);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);


    // 绘制电池边框
    QPen pen(colorBorder);
    pen.setWidth(2);
    pen.setStyle(Qt::SolidLine);
    pen.setCapStyle(Qt::FlatCap);
    pen.setJoinStyle(Qt::BevelJoin);
    painter.setPen(pen);

    QBrush brush(colorBack);
    brush.setStyle(Qt::SolidPattern);
    painter.setBrush(brush);
    rect.setRect(1,1,109,48);
    painter.drawRect(rect);                     // 绘制电池边框
    brush.setColor(colorBorder);
    painter.setBrush(brush);
    rect.setRect(110,15,10,20);
    painter.drawRect(rect);                     // 画电池正极头


    // 画电量柱
    if(m_powerLevel>m_warnLevel){
        brush.setColor(colorPower);
        pen.setColor(colorPower);
    }
    else{
        brush.setColor(colorWarning);
        pen.setColor(colorWarning);
    }

    painter.setBrush(brush);
    painter.setPen(pen);
    if(m_powerLevel>0){
        rect.setRect(5,5,m_powerLevel,40);
        painter.drawRect(rect);
    }

    // 绘制电量百分比文字
    QFontMetrics textSize(this->font());
    QString powStr=QString::asprintf("%d%%",m_powerLevel);
    QRect textRect=textSize.boundingRect(powStr);
    painter.setFont(this->font());
    pen.setColor(colorBorder);
    painter.setPen(pen);
    painter.drawText(55-textRect.width()/2,23+textRect.height()/2,powStr);

    event->accept();
}
