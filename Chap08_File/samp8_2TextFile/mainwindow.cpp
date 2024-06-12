#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSaveFile>
#include <QException>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getFile(bool save)
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString dlgTitle="选择一个文件";
    QString filter="程序文件(*.h *.cpp);; 文本文件(*.txt);; 所有文件(*.*)";
    QString fileName;
    if(save){
        fileName=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);
    }
    else
        fileName= QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if(fileName.isEmpty())
        return fileName;

    QFileInfo fileinfo(fileName);
    QDir::setCurrent(fileinfo.absoluteFilePath());
    return fileName;


}

void MainWindow::on_actOpen_IODevice_triggered()
{
    // 用只读的方式打开文件
    QFile aFile(getFile());
    if(!aFile.exists())
        return;

    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    ui->textEditDevice->appendPlainText(aFile.readAll());
    aFile.close();
}


void MainWindow::on_actSave_IODevice_triggered()
{
    QFile aFile(getFile(true));
    if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return;

    QString str=ui->textEditDevice->toPlainText();
    aFile.write(str.toUtf8(),str.toUtf8().length());
    aFile.close();
}


void MainWindow::on_actSave_TextSafe_triggered()
{
    QSaveFile aFile(getFile(true));
    if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return;

    aFile.setDirectWriteFallback(false);  // 写临时文件，直接写入
    try{
        QString str=ui->textEditDevice->toPlainText();
        aFile.write(str.toUtf8(),str.toUtf8().length());
        aFile.commit();
    }
    catch(QException &e){
        QMessageBox::warning(this,"异常","保存文件过程方式错误！");
        aFile.cancelWriting();
    }
}


void MainWindow::on_actOpen_TextStream_triggered()
{
    QFile aFile(getFile());
    if(!aFile.exists())
        return;

    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    QTextStream aStream(&aFile);
    aStream.setAutoDetectUnicode(true);
    while(!aStream.atEnd()){
        ui->textEditStream->appendPlainText(aStream.readLine());
    }

    aFile.close();
}

#include <QTextBlock>
void MainWindow::on_actSave_QTextStream_triggered()
{
    QSaveFile aFile(getFile(true));
    if(!aFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return;

    aFile.setDirectWriteFallback(false);  // 写临时文件，直接写入
    try{
        QTextStream aStream(&aFile);
        aStream.setAutoDetectUnicode(true);
        QTextDocument *doc= ui->textEditStream->document();
        for(int i=0;i<doc->blockCount();++i){
            aStream<<doc->findBlockByLineNumber(i).text();
            aStream<<"\n";
        }

        aFile.commit();
    }
    catch(QException &e){
        QMessageBox::warning(this,"异常","保存文件过程方式错误！");
        aFile.cancelWriting();
    }
}

