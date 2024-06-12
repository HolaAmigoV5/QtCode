#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QProgressDialog>
#include <QElapsedTimer>

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


void Widget::on_btnOpen_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString fileName= QFileDialog::getOpenFileName(this,"选择一个文件",curPath,"文本文件(*.txt);;图片(*.jpg *.gif *.png);;所有文件(*.*)");
    if(!fileName.isEmpty())
        ui->plainTextEdit->appendPlainText(fileName);
}


void Widget::on_btnOpenMulti_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QStringList fileNames= QFileDialog::getOpenFileNames(this,"选择多个文件",curPath,"文本文件(*.txt);;图片(*.jpg *.gif *.png);;所有文件(*.*)");
    for(const auto &fileName:fileNames)
        ui->plainTextEdit->appendPlainText(fileName);
}

void Widget::on_btnSelDir_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString selectedDir=QFileDialog::getExistingDirectory(this,"选择一个目录",curPath);
    if(!selectedDir.isEmpty())
        ui->plainTextEdit->appendPlainText(selectedDir);
}


void Widget::on_btnSave_clicked()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString fileName=QFileDialog::getSaveFileName(this,"保存文件",curPath,"文本文件(*.txt);;图片(*.jpg *.gif *.png);;所有文件(*.*)");

    if(!fileName.isEmpty())
        ui->plainTextEdit->appendPlainText(fileName);
}


void Widget::on_btnColor_clicked()
{
    QPalette pal=ui->plainTextEdit->palette();
    QColor iniColor=pal.color(QPalette::Text);
    QColor color=QColorDialog::getColor(iniColor,this,"选择颜色");
    if(color.isValid()){
        pal.setColor(QPalette::Text,color);
        ui->plainTextEdit->setPalette(pal);
    }
}


void Widget::on_btnFont_clicked()
{
    QFont iniFont=ui->plainTextEdit->font();
    bool ok=false;
    QFont font=QFontDialog::getFont(&ok,iniFont);
    if(ok)
        ui->plainTextEdit->setFont(font);
}


void Widget::on_btnProgress_clicked()
{
    int minValue=0, maxValue=200;
    QProgressDialog dlgProgress("正在复制文件...", "取消",minValue,maxValue,this);
    dlgProgress.setWindowTitle("复制文件");
    dlgProgress.setWindowModality(Qt::WindowModal);
    connect(&dlgProgress,&QProgressDialog::canceled, this, [&](){
        ui->plainTextEdit->appendPlainText("用户已经取消了！");
    });

    QElapsedTimer msCounter;
    for(int i=minValue; i<=maxValue; ++i){
        dlgProgress.setValue(i);
        dlgProgress.setLabelText(QString::asprintf("正在复制文件，第%d",i));

        msCounter.start();
        while (true) {
            if(msCounter.elapsed()>30)
                break;

        }

        if(dlgProgress.wasCanceled())
            break;
    }
}

#include <QInputDialog>
void Widget::on_btnInputString_clicked()
{
    QString dlgTitle="输入文字对话框";
    QString txtLabel="请输入文件名";
    QString iniInput="新建文件.txt";
    QLineEdit::EchoMode echoMode=QLineEdit::Normal;
    bool ok=false;
    QString text=QInputDialog::getText(this, dlgTitle,txtLabel,echoMode,iniInput,&ok);
    if(ok && !text.isEmpty()){
        ui->plainTextEdit->appendPlainText(text);
    }
}


void Widget::on_btnInputInt_clicked()
{
    QString dlgTitle="输入整数对话框";
    QString txtLabel="设置文本框字体大小";
    int defaultValue=ui->plainTextEdit->font().pointSize();
    int minValue=6, maxValue=60, stepValue=1;

    bool ok=false;
    int inputValue=QInputDialog::getInt(this, dlgTitle,txtLabel,defaultValue,minValue,maxValue,stepValue,&ok);

    if(ok){
        QString str=QString("文本框字体大小被设置为：%1").arg(inputValue);
        ui->plainTextEdit->appendPlainText(str);
        QFont font=ui->plainTextEdit->font();
        font.setPointSize(inputValue);
        ui->plainTextEdit->setFont(font);
    }
}


void Widget::on_btnInputFloat_clicked()
{
    QString dlgTitle="输入浮点数对话框";
    QString txtLabel="输入一个浮点数";
    float defaultValue=3.13;
    float minValue=0, maxValue=10000;
    int decimals=2;

    bool ok=false;
    float inputValue=QInputDialog::getDouble(this, dlgTitle,txtLabel,defaultValue,minValue,maxValue,decimals,&ok);

    if(ok){
        QString str=QString::asprintf("输入一个浮点数：%.2f", inputValue);
        ui->plainTextEdit->appendPlainText(str);
    }
}


void Widget::on_btnInputItem_clicked()
{
    QStringList items;
    items<<"优"<<"良"<<"中"<<"差";
    QString dlgTitle="输入项对话框", txtLabel="请选择一个等级";
    int curIndex=0;
    bool editable=false;
    bool ok=false;

    QString text=QInputDialog::getItem(this,dlgTitle,txtLabel,items,curIndex, editable,&ok);
    if(ok && !text.isEmpty())
        ui->plainTextEdit->appendPlainText(text);
}

#include <QMessageBox>
void Widget::on_btnMsgQuestion_clicked()
{
    int result= QMessageBox::question(this,"Question消息框","文件已经修改，是否需要保存？",
                          QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if(result==QMessageBox::Yes){
        ui->plainTextEdit->appendPlainText("Question消息框：Yes被选中");
    }
    else
        ui->plainTextEdit->appendPlainText("Question消息框：No被选中");
}

