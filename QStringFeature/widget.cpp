#include "widget.h"
#include "ui_widget.h"

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

void Widget::on_btnFontBack_clicked()
{
    ui->plainTextEdit->appendPlainText("front(), back() 函数测试");
    QString str1=ui->comboBoxStr1->currentText();
    ui->plainTextEdit->appendPlainText(str1);
    ui->plainTextEdit->appendPlainText("font = "+QString(str1.front()));
    ui->plainTextEdit->appendPlainText("back = "+QString(str1.back()));
}


void Widget::on_btnLeftRight_clicked()
{
    ui->plainTextEdit->appendPlainText("left(), right() 函数测试");
    QString str1=ui->comboBoxStr1->currentText();
    ui->plainTextEdit->appendPlainText(str1);
    ui->plainTextEdit->appendPlainText("left = "+QString(str1.left(2)));
    ui->plainTextEdit->appendPlainText("right = "+QString(str1.right(3)));
}

void Widget::on_btnFirstLast_clicked()
{
    ui->plainTextEdit->appendPlainText("First(), Last() 函数测试");
    QString str1=ui->comboBoxStr1->currentText();
    QString str2=ui->comboBoxStr2->currentText();
    ui->plainTextEdit->appendPlainText("str1:"+str1);
    ui->plainTextEdit->appendPlainText("str2:"+str2);
    int N= str1.lastIndexOf(str2);
    QString strFirst=str1.first(N+1);
    QString strLast=str1.last(str1.size()-N-1);


    ui->plainTextEdit->appendPlainText("路径名称 : "+strFirst);
    ui->plainTextEdit->appendPlainText("文件名称 : "+strLast);
}

void Widget::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}


void Widget::on_btnSection_clicked()
{
    ui->plainTextEdit->appendPlainText("Section() 函数测试");
    QString str1=ui->comboBoxStr1->currentText();
    QString str2=ui->comboBoxStr2->currentText();

    int N=ui->spinBox->value();
    QString strSection=str1.section(str2,N,N);
    ui->plainTextEdit->appendPlainText(strSection);
}


void Widget::on_btnisNullEmpty_clicked()
{
    ui->plainTextEdit->appendPlainText("isNull和isEmpty函数测试：");
    QString str1, str2="";
    ui->plainTextEdit->appendPlainText("QString str1, Str2=""");
    QString str="isNull()=";
    str=(str1.isNull())?str+"True":str+"False";
    ui->plainTextEdit->appendPlainText(str);

    str="isEmpty()=";
    str=(str2.isEmpty())?str+"True":str+"False";
    ui->plainTextEdit->appendPlainText(str);
}


void Widget::on_btnResize_clicked()
{
    ui->plainTextEdit->appendPlainText("resize函数测试：");
    QString str;
    str.resize(5,'0');
    ui->plainTextEdit->appendPlainText(str);

    str.resize(10, QChar(0x54C8));
    ui->plainTextEdit->appendPlainText(str);
}


void Widget::on_btnSizeLength_clicked()
{
    ui->plainTextEdit->appendPlainText("size, length函数测试：");
    QString str1=ui->comboBoxStr1->currentText();
    ui->plainTextEdit->appendPlainText(QString::asprintf("size: %lld",str1.size()));
    //ui->plainTextEdit->appendPlainText(QString::asprintf("count: %lld",str1.count()));
    ui->plainTextEdit->appendPlainText(QString::asprintf("length: %lld",str1.length()));
}


void Widget::on_btnFill_clicked()
{
    ui->plainTextEdit->appendPlainText("Fill 函数测试：");
    QString str="Hello";
    ui->plainTextEdit->appendPlainText("原来字符串："+str);
    str.fill('X');
    ui->plainTextEdit->appendPlainText("fill('X')后的字符串："+str);
    str.fill('A',2);
    ui->plainTextEdit->appendPlainText("fill('A',2)后字符串："+str);
    str.fill(QChar(0x54C8),3);
    ui->plainTextEdit->appendPlainText("fill(QChar(0x54C8)后字符串："+str);
}


void Widget::on_btnTrimmed_clicked()
{
    ui->plainTextEdit->appendPlainText("trime, simplified 函数测试：");
    QString str1=ui->comboBoxStr1->currentText();
    ui->plainTextEdit->appendPlainText("str1 = "+str1);

    QString str2=str1.trimmed();
    ui->plainTextEdit->appendPlainText("Trimmed() = "+str2);

    str2=str1.simplified();
    ui->plainTextEdit->appendPlainText("simplified() = "+str2);
}

