﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tformdoc.h"


#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->mdiArea);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetActEnabled(bool flag){
    ui->actCut->setEnabled(flag);
    ui->actCopy->setEnabled(flag);
    ui->actPaste->setEnabled(flag);
    ui->actFont->setEnabled(flag);
}

// 新建
void MainWindow::on_actDoc_New_triggered()
{
    TFormDoc *formDoc=new TFormDoc(this);
    ui->mdiArea->addSubWindow(formDoc);
    formDoc->show();

    SetActEnabled(true);
}

// 打开文件
void MainWindow::on_actDoc_Open_triggered()
{
    bool needNew=true;
    TFormDoc *formDoc;
    if(ui->mdiArea->subWindowList().size()>0){
        formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
        needNew=!(formDoc->isFileOpened());
    }
    QString curPath=QCoreApplication::applicationDirPath();
    QString aFileName=QFileDialog::getOpenFileName(this,"打开一个文件",curPath,"C程序文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)");
    if(!aFileName.isEmpty()){
        if(needNew){
            formDoc=new TFormDoc(this);
            ui->mdiArea->addSubWindow(formDoc);
        }
        formDoc->loadFromFile(aFileName);
        formDoc->show();

        SetActEnabled(true);
    }
}


void MainWindow::on_actFont_triggered()
{
    TFormDoc *formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
    formDoc->setEditFont();
}


void MainWindow::on_actCut_triggered()
{
    TFormDoc *formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
    formDoc->textCut();
}


void MainWindow::on_actCopy_triggered()
{
    TFormDoc *formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
    formDoc->textCopy();
}


void MainWindow::on_actPaste_triggered()
{
    TFormDoc *formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
    formDoc->textPaste();
}


void MainWindow::on_actViewMode_triggered(bool checked)
{
    if(checked){
        ui->mdiArea->setViewMode(QMdiArea::TabbedView);
    }
    else
        ui->mdiArea->setViewMode(QMdiArea::SubWindowView);

    ui->mdiArea->setTabsClosable(checked);
    ui->actCascade->setEnabled(!checked);
    ui->actTile->setEnabled(!checked);
}


void MainWindow::on_actCascade_triggered()
{
    ui->mdiArea->cascadeSubWindows();
}


void MainWindow::on_actTile_triggered()
{
    ui->mdiArea->tileSubWindows();
}


void MainWindow::on_actClossAll_triggered()
{
    ui->mdiArea->closeAllSubWindows();
}


void MainWindow::on_actDoc_Save_triggered()
{
    TFormDoc *formDoc=dynamic_cast<TFormDoc*>(ui->mdiArea->activeSubWindow()->widget());
    formDoc->saveToFile();
}


void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *arg1)
{
    Q_UNUSED(arg1);
    if(ui->mdiArea->subWindowList().size()==0){
        SetActEnabled(false);
    }
}

