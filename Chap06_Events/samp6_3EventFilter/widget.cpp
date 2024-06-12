#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->labDC->installEventFilter(this);
    ui->labSC->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->labSC)
    {
        if(event->type()==QEvent::Enter)
            ui->labSC->setStyleSheet("background-color:rgb(170,255,255);");
        else if(event->type()==QEvent::Leave){
            ui->labSC->setText("单击我啊");
            ui->labSC->setStyleSheet("");
        }
        else if(event->type()==QEvent::MouseButtonPress)
            ui->labSC->setText("button pressed");
        else if(event->type()==QEvent::MouseButtonRelease)
            ui->labSC->setText("button Release");
    }
    else if(watched==ui->labDC){
        if(event->type()==QEvent::Enter)
            ui->labDC->setStyleSheet("background-color:rgb(85,255,127);");
        else if(event->type()==QEvent::Leave){
            ui->labDC->setText("双击我啊");
            ui->labDC->setStyleSheet("");
        }
        else if(event->type()==QEvent::MouseButtonDblClick)
            ui->labDC->setText("DoubleClick");
    }

    return QWidget::eventFilter(watched,event);
}
