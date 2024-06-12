#include "qmylabel.h"
#include <QPalette>

QMyLabel::QMyLabel(QWidget *parent):QLabel(parent) {}


void QMyLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    emit doubleClicked();
}

void QMyLabel::enterEvent(QEnterEvent *event)
{
    QPalette plet=this->palette();
    QFont font= this->font();
    font.setPointSize(20);
    font.setBold(true);
    setFont(font);
    plet.setColor(QPalette::WindowText,Qt::red);
    setPalette(plet);

    this->adjustSize();
    QLabel::enterEvent(event);
}

void QMyLabel::leaveEvent(QEvent *event)
{
    QPalette plet=this->palette();
    plet.setColor(QPalette::WindowText,Qt::black);
    setPalette(plet);

    QFont font= this->font();
    font.setPointSize(12);
    font.setBold(false);
    setFont(font);
    adjustSize();

    QLabel::leaveEvent(event);
}
