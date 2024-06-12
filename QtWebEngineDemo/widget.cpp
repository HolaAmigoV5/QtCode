#include "widget.h"
#include "ui_widget.h"
#include <QScreen>
#include <QProcess>
#include <QDebug>
#include <QFile>

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

bool Widget::CallNginxExe()
{
    QString exePath="D:/Working/CY/demo/nginx/nginx.exe";
    return QProcess::startDetached(exePath);
}

bool Widget::CheckAppRunningStatus(const QString &appName)
{
#ifdef Q_OS_WIN
    QProcess* process = new QProcess;
    process->start("tasklist" ,QStringList()<<"/FI"<<"imagename eq " +appName);
    process->waitForFinished();
    QString outputStr = QString::fromLocal8Bit(process->readAllStandardOutput());
    if(outputStr.contains(appName)){
        return true;
    }
    else{
        return false;
    }
#endif
}



void Widget::on_pushButton_clicked()
{
    if(!CheckAppRunningStatus("nginx.exe")){
        if(CallNginxExe())
            qDebug()<<"启动nginx.exe成功";
        else
            qDebug()<<"启动nginx.exe失败";
    }

    // QRect screenRect=QGuiApplication::primaryScreen()->geometry();
    // this->resize(screenRect.width(),screenRect.height());
    mainView=new QWebEngineView(this);
    mainView->load(QUrl("http://localhost:7503"));
    ui->vLayout->addWidget(mainView);
}

