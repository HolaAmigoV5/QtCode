#include "smallwidget.h"
#include "ui_smallwidget.h"

smallWidget::smallWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::smallWidget)
{
    ui->setupUi(this);

    void(QSpinBox::*spSignal)(int)=&QSpinBox::valueChanged;
    connect(ui->spinBox, spSignal, ui->horizontalSlider,&QSlider::setValue);

    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBox, &QSpinBox::setValue);
}

smallWidget::~smallWidget()
{
    delete ui;
}

int smallWidget::GetCurrentValue(){
    return ui->spinBox->value();
}

void smallWidget::SetHalfValue(int num){
    ui->spinBox->setValue(num);
}
