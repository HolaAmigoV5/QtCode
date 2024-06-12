#include "widget.h"
#include "ui_widget.h"
#include <QPushButton>
#include <QFileDialog>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QDateTime>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 点击选取文件按钮，弹出文件对话框
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        QString path= QFileDialog::getOpenFileName(this,"打开文件","C:\\Users\\LFive\\Desktop");

        // 将路径放入到lineEdit中
        ui->lineEdit->setText(path);

        // 读取内容，放入到textEdit中。QFile默认格式是utf-8
        QFile file(path);
        file.open(QIODevice::ReadOnly);
        QByteArray array= file.readAll();
        ui->textEdit->setText(QString(array));
        file.close();


        // 进行写文件
//        file.open(QIODevice::Append);   // 追加的方式写
//        file.write("1231413");
//        file.close();


        // QFileInfo 文件信息类
        QFileInfo info(path);
        qDebug()<<"大小："<<info.size()<< " 后缀名："<<info.suffix()<<" 文件名称：" <<info.fileName() <<" 文件路径："<<info.filePath();
        qDebug()<<"文件日期："<<info.created().toString("yyyy-MM-dd hh:mm:ss");
        qDebug()<<"最后修改日期："<<info.lastModified().toString("yyyy/MM/dd hh:mm:ss");

    });
}

Widget::~Widget()
{
    delete ui;
}

