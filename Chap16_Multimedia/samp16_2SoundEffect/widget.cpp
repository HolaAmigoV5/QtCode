#include "widget.h"
#include "ui_widget.h"

#include <QPainter>

void Widget::defense(QString weapon)
{
    QUrl url=QUrl::fromLocalFile(appPath+"/sound/"+weapon);
    qDebug()<<"Defense: "<<url.toString();
    player1->setSource(url);
    qDebug()<<(player1->source().toString());
    player1->play();
}

void Widget::attack(QString weapon)
{
    player2->stop();
    QUrl url=QUrl::fromLocalFile(appPath+"/sound/"+weapon);
    qDebug()<<"attack"<<url.toString();
    player2->setSource(url);
    qDebug()<<(player2->source().toString());
    player2->play();
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0,0,this->width(),this->height(),pixBackground);
    event->accept();
}

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    appPath=QCoreApplication::applicationDirPath();
    pixBackground.load(appPath+"/sound/background.jpg");

    player1=new QSoundEffect(this);
    //player1->setLoopCount(3);

    player2=new QSoundEffect(this);
    //player2->setLoopCount(3);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_DAK47_clicked()
{
    defense("Ak7.wav");
}


void Widget::on_btn_DMachinegun_clicked()
{
    defense("machinegun.wav");
}


void Widget::on_btn_DFire_clicked()
{
    defense("fire.wav");
}


void Widget::on_btn_DShell_clicked()
{
    defense("shell.wav");
}


void Widget::on_btn_DBlast1_clicked()
{
    defense("blast.wav");
}


void Widget::on_btn_DBlast2_clicked()
{
    defense("blast2.wav");
}


void Widget::on_btn_A_Ak47_clicked()
{
    attack("Ak7.wav");
}


void Widget::on_btn_A_Machinegun_clicked()
{
    attack("machinegun.wav");
}


void Widget::on_btn_A_fire2_clicked()
{
    attack("fire2.wav");
}


void Widget::on_btn_A_Mine_clicked()
{
    attack("mine.wav");
}


void Widget::on_btn_A_blast_clicked()
{
    attack("blast.wav");
}


void Widget::on_btn_A_Tank_clicked()
{
    attack("tank.wav");
}

