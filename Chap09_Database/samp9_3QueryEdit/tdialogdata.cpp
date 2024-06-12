#include "tdialogdata.h"
#include "ui_tdialogdata.h"

#include <QFileDialog>

TDialogData::TDialogData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDialogData)
{
    ui->setupUi(this);
}

TDialogData::~TDialogData()
{
    delete ui;
}

QSqlRecord TDialogData::record()
{
    m_record.setValue("empNo",ui->dbSpinEmpNo->value());
    m_record.setValue("Name",ui->dbEditName->text());
    m_record.setValue("Gender",ui->dbComboSex->currentText());
    m_record.setValue("Birthday",ui->dbEditBirth->date());

    m_record.setValue("Province",ui->dbComboProvince->currentText());
    m_record.setValue("Department",ui->dbComboDep->currentText());

    m_record.setValue("Salary",ui->dbSpinSalary->value());
    m_record.setValue("Memo",ui->editMemo->toPlainText());
    return m_record;
}

void TDialogData::setRecord(const QSqlRecord &newRecord)
{
    m_record = newRecord;
    ui->dbSpinEmpNo->setEnabled(false);
    setWindowTitle("Data对话框");

    ui->dbSpinEmpNo->setValue(newRecord.value("empNo").toInt());
    ui->dbEditName->setText(newRecord.value("Name").toString());
    ui->dbComboSex->setCurrentText(newRecord.value("Gender").toString());
    ui->dbEditBirth->setDate(newRecord.value("Birthday").toDate());
    ui->dbComboProvince->setCurrentText(newRecord.value("Province").toString());
    ui->dbComboDep->setCurrentText(newRecord.value("Department").toString());
    ui->dbSpinSalary->setValue(newRecord.value("Salary").toInt());
    ui->editMemo->setPlainText(newRecord.value("Memo").toString());

    QVariant va=newRecord.value("Photo");
    if(va.isValid()){
        QPixmap pic;
        pic.loadFromData(va.toByteArray());
        ui->LabPhoto->setPixmap(pic.scaledToWidth(ui->LabPhoto->width()));
    }
    else{
        ui->LabPhoto->clear();
    }
}

void TDialogData::on_btnInput_clicked()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择图片文件","","JPG照片(*.jpg);;PNG照片(*.png)");
    if(aFile.isEmpty())
        return;

    QFile file(aFile);
    if(!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data=file.readAll();
    file.close();


    m_record.setValue("Photo",data);
    QPixmap pix;
    pix.loadFromData(data);
    ui->LabPhoto->setPixmap(pix.scaledToWidth(ui->LabPhoto->width()));
}


void TDialogData::on_btnClearPhoto_clicked()
{
    m_record.setNull("Photo");
    ui->LabPhoto->clear();
}

