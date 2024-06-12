#include "widget.h"
#include "ui_widget.h"
#include <QMetaProperty>
#include <QMetaClassInfo>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    boy=new TPerson("小明",this);
    boy->setAge(10);
    boy->setProperty("sex","男孩");

    girl=new TPerson("小丽",this);
    girl->setAge(20);
    girl->setProperty("sex","女孩");

    // 动态添加属性
    ui->spinBoy->setProperty("isBoy", true);
    ui->spinGirl->setProperty("isBoy",false);

    connect(boy,SIGNAL(ageChanged(int)), this, SLOT( do_ageChanged(int)));
    connect(girl,SIGNAL(ageChanged(int)), this, SLOT( do_ageChanged(int)));

    connect(ui->spinBoy,SIGNAL(valueChanged(int)), this, SLOT( do_spinChanged(int)));
    connect(ui->spinGirl,SIGNAL(valueChanged(int)), this, SLOT( do_spinChanged(int)));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::do_ageChanged(int value)
{
    TPerson *person= qobject_cast<TPerson*>(sender());
    QString str = QString("姓名：%3，性别：%1，年龄：%2").arg(person->property("sex").toString())
                      .arg(value).arg(person->property("name").toString());

    if(person->property("sex")=="男孩"){
        ui->spinBoy->setValue(value);
    }
    else{
        ui->spinGirl->setValue(value);
    }
    ui->plainTextEdit->appendPlainText(str);
}

void Widget::do_spinChanged(int value)
{
    QSpinBox *spinBox= qobject_cast<QSpinBox*>(sender());
    QString str;
    if(spinBox->property("isBoy").toBool()){
        boy->setAge(value);
        str=QString("男孩年龄是：%1").arg(value);
    }
    else{
        girl->setAge(value);
        str=QString("女孩年龄是：%1").arg(value);
    }
    ui->plainTextEdit->appendPlainText(str);
}

void Widget::on_btnBoyInc_clicked()
{
    boy->incAge();
}


void Widget::on_btnGirlInc_clicked()
{
    girl->incAge();
}


void Widget::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}


void Widget::on_btnClassInfo_clicked()
{
    const QMetaObject* meta=boy->metaObject();
    ui->plainTextEdit->appendPlainText(QString("类名称：%1\n").arg(meta->className()));

    ui->plainTextEdit->appendPlainText("属性：");
    for (int i = meta->propertyOffset(); i < meta->propertyCount(); ++i) {
        const char* propName=meta->property(i).name();
        QString propValue=boy->property(propName).toString();
        ui->plainTextEdit->appendPlainText(QString("属性名称=%1，属性值=%2").arg(propName,propValue));
    }

    ui->plainTextEdit->appendPlainText("\n类信息(classInfo)：");
    for (int i = meta->classInfoOffset(); i < meta->classInfoCount(); ++i) {
        QMetaClassInfo classInfo=meta->classInfo(i);
        ui->plainTextEdit->appendPlainText(QString("Name=%1，Value=%2").arg(classInfo.name(),classInfo.value()));
    }
}

