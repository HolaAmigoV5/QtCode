#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 使用添加Qt资源：前缀名+文件名
    ui->actionnew->setIcon(QIcon(":/Images/jt.png"));
    ui->actionopen->setIcon(QIcon(":/Images/mlo.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

