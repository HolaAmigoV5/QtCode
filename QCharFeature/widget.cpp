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


void Widget::setChecked(QChar ch){
    ui->chkDigit->setChecked(ch.isDigit());     // 是否为数字
    ui->chkLetter->setChecked(ch.isLetter());   // 是否为字母
    ui->chkLetterOrNumber->setChecked(ch.isLetterOrNumber());
    ui->chkLower->setChecked(ch.isLower());     // 是否小写
    ui->chkUpper->setChecked(ch.isUpper());     // 是否是大写
    ui->chkMark->setChecked(ch.isMark());       // 是否为符号
    ui->chkPunct->setChecked(ch.isPunct());     // 是否为标点符号
    ui->chkSpace->setChecked(ch.isSpace());     // 是否为空白符
    ui->chkSymbol->setChecked(ch.isSymbol());   // 是否为符号
}

void Widget::on_btnGetChars_clicked()
{
    QString str=ui->editStr->text();
    if(!str.isEmpty()){
        for (int i = 0; i < str.size(); ++i) {
            QChar ch=str.at(i);
            char16_t uniCode=ch.unicode();

            QString chStr(ch);
            QString info=chStr+QString::asprintf(",\tUnicode编码=0x%X", uniCode);
            ui->plainTextEdit->appendPlainText(info);
        }
    }
}


void Widget::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}


void Widget::on_btnCharJudge_clicked()
{
    QString str=ui->editChar->text();
    if(!str.isEmpty()){
        QChar ch=str.at(0);
        char16_t uniCode=ch.unicode();
        QString info=str+QString::asprintf(",\tUnicode编码=0x%X", uniCode);
        ui->plainTextEdit->appendPlainText(info);

        setChecked(ch);
    }
}

void Widget::on_btnConvLatin1_clicked()
{
    QString str="Dimple";
    ui->plainTextEdit->appendPlainText(str);
    QChar chP=QChar::fromLatin1('P');
    str[0]=chP;
    ui->plainTextEdit->appendPlainText("\n"+str);
}


void Widget::on_btnConvUTF16_clicked()
{
    QString str="Hello，北京";
    QString wh="武汉";
    str[6]=wh.at(0);/*QChar(0x9752)*/;
    str[7]=wh.at(1);/*QChar(0x5c9B)*/;

    ui->plainTextEdit->appendPlainText("\n"+str);
}


void Widget::on_btnCompare_clicked()
{
    QString HuStr="河to湖";
    QChar he=QChar::fromUcs2(HuStr[0].unicode());
    QChar hu=QChar(HuStr[3].unicode());
    QString str="他们来自河南或河北";
    ui->plainTextEdit->appendPlainText(str);
    for (int i = 0; i < str.size(); ++i) {
        if(str.at(i)==he)
            str[i]=hu;
    }

    ui->plainTextEdit->appendPlainText("\n"+ str);
}

