#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->tabWidget);

    ui->tabWidget->setVisible(false);
    ui->tabWidget->clear();
    ui->tabWidget->setTabsClosable(true);
    //setWindowState(Qt::WindowMaximized);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actWidgetInside_triggered()
{
    TFormDoc *formDoc=new TFormDoc(this);
    formDoc->setAttribute(Qt::WA_DeleteOnClose);
    int cur= ui->tabWidget->addTab(formDoc, QString::asprintf("Doc %d", ui->tabWidget->count()));
    ui->tabWidget->setCurrentIndex(cur);
    ui->tabWidget->setVisible(true);

    connect(formDoc,&TFormDoc::titleChanged,this, &MainWindow::do_changeTabTitle);
}

void MainWindow::do_changeTabTitle(QString str)
{
    int index=ui->tabWidget->currentIndex();
    ui->tabWidget->setTabText(index,str);
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    ui->tabWidget->widget(index)->close();
}

#include <QPainter>
void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.drawPixmap(0,ui->toolBar->height(),width(),
                       height()-ui->toolBar->height()-ui->statusbar->height(),
                       QPixmap(":/images/back2.jpg"));
}

// 独立的Widget
void MainWindow::on_actWidget_triggered()
{
    TFormDoc *formDoc=new TFormDoc();
    formDoc->setAttribute(Qt::WA_DeleteOnClose);
    formDoc->setWindowTitle("基于QWidget的窗口，无parent");
    formDoc->setWindowOpacity(0.8);
    formDoc->show();
}

