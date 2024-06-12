#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 点击新建后，弹出对话框
    connect(ui->actionnew, &QAction::triggered,[=](){
        // 对话框：模态和非模态
//        QDialog dlg(this);
//        dlg.resize(400,400);
//        dlg.exec();     // 模态对话框

//        qDebug()<<"模态对话框弹出了";

        // 非模态对话框
//        QDialog *dlg2 =new QDialog(this);
//        dlg2->resize(400,400);
//        dlg2->show();
//        dlg2->setAttribute(Qt::WA_DeleteOnClose);      // 关闭后销毁
//        qDebug()<<"非模态对话框弹出了";

        // 消息对话框
        QMessageBox::critical(this,"错误", "请注意，出错了！");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

