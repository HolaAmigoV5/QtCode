#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 创建一个老师对象
    this->zt=new Teacher(this);


    // 创建一个学生对象
    this->st=new Student(this);

    // 老师饿了，学生请客
//    connect(zt,&Teacher::hungry, st, &Student::treat);
//    ClassIsOver();

    void(Teacher:: *teacherSignal)(QString) = &Teacher::hungry;
    void(Student:: *studentSlot)(QString) = &Student::treat;
    connect(zt,teacherSignal,st,studentSlot);
    //ClassIsOver();

    QPushButton *btn=new QPushButton("下课",this);
    btn->move(200,200);
    this->resize(800,600);

    // 点击按钮，触发下课
    //connect(btn,&QPushButton::clicked, this, &Widget::ClassIsOver);

    // 无参信号和槽连接
    void(Teacher:: *teacherSignal2)(void) = &Teacher::hungry;
    void(Student:: *studentSlot2)(void) = &Student::treat;
    connect(zt,teacherSignal2,st,studentSlot2);

    connect(btn,&QPushButton::clicked, zt, teacherSignal2);

    // 断开信号
    disconnect(zt,teacherSignal2,st,studentSlot2);

    QPushButton *myBtn=new QPushButton("按钮1",this);
    QPushButton *myBtn2=new QPushButton("按钮2",this);
    myBtn2->move(100,100);

    int m=10;
    connect(myBtn, &QPushButton::clicked, this, [m]()mutable {m+=100; qDebug()<<m;});
    connect(myBtn2, &QPushButton::clicked, this, [=](){qDebug()<<m;});
    qDebug()<<m;
}

void Widget::ClassIsOver(){
    // 下课函数，调用后，触发老师饿了的信号。
    //emit zt->hungry();
    emit zt->hungry("包子");
}

Widget::~Widget()
{
    delete ui;
}

