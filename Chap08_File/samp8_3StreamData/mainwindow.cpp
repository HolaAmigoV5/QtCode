#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QFontDialog>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->gBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

template<class T>
void MainWindow::writeByStream(T Value){
    QFile fileDevice(m_filename);
    if(!fileDevice.open(QIODevice::WriteOnly))
        return;

    QDataStream fileStream(&fileDevice);
    fileStream.setVersion(QDataStream::Qt_6_6);
    if(ui->radio_BigEndian->isChecked())
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if(ui->radio_Single->isChecked())
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    fileStream<<Value;
    fileDevice.close();
}

template<class T>
void MainWindow::readByStream(T &Value){
    if(!QFile::exists(m_filename))
        return;

    QFile fileDevice(m_filename);
    if(!fileDevice.open(QIODevice::ReadOnly))
        return;

    QDataStream fileStream(&fileDevice);
    fileStream.setVersion(QDataStream::Qt_6_6);
    if(ui->radio_BigEndian->isChecked())
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if(ui->radio_Single->isChecked())
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    fileStream>>Value;
    fileDevice.close();
}

#include <QFileDialog>
void MainWindow::on_btnFile_clicked()
{
    m_filename=QFileDialog::getSaveFileName(this,"选择一个文件",QApplication::applicationDirPath(),"数据流文件(*.tream)");
    if(!m_filename.isEmpty())
    {
        ui->editFilename->setText(m_filename);
        ui->actSaveALL->setEnabled(true);
        ui->actReadALL->setEnabled(true);
        ui->gBox->setEnabled(true);
    }
}


void MainWindow::on_btnColor_In_clicked()
{
    QPalette plet=ui->btnColor_In->palette();
    QColor color=plet.buttonText().color();
    color=QColorDialog::getColor(color);
    if(color.isValid()){
        plet.setColor(QPalette::ButtonText, color);
        ui->btnColor_In->setPalette(plet);
    }
}


void MainWindow::on_btnFont_In_clicked()
{
    QFont font=ui->btnFont_In->font();
    bool ok=false;
    font=QFontDialog::getFont(&ok,font,this);
    if(ok){
        ui->btnFont_In->setFont(font);
    }
}


void MainWindow::on_btnInt8_Write_clicked()
{
    qint8 value=ui->spinInt8->value();
    writeByStream(value);
}


void MainWindow::on_btnInt8_Read_clicked()
{
    qint8 value=0;
    readByStream(value);
    ui->editInt8->setText(QString::number(value));
}


void MainWindow::on_btnUint8_Write_clicked()
{
    quint8 value=ui->spinUint8->value();
    writeByStream(value);
}


void MainWindow::on_btnUint8_Read_clicked()
{
    quint8 value=0;
    readByStream(value);
    ui->editUint8->setText(QString("%1").arg(value));
}


void MainWindow::on_btnBool_Write_clicked()
{
    bool value=ui->chkBox_In->isChecked();
    writeByStream(value);
}


void MainWindow::on_btnBool_Read_clicked()
{
    bool value=false;
    readByStream(value);
    ui->chkBox_Out->setChecked(value);
}


void MainWindow::on_btnDouble_Write_clicked()
{
    double value=ui->spinDouble->value();
    writeByStream(value);
}


void MainWindow::on_btnDouble_Read_clicked()
{
    double value=0.0;
    readByStream(value);
    ui->editDouble->setText(QString::asprintf("%1.4f",value));
}


void MainWindow::on_btnFontWrite_clicked()
{
    QFont font=ui->btnFont_In->font();
    writeByStream(font);
}


void MainWindow::on_btnFont_Read_clicked()
{
    QFont font;
    readByStream(font);
    ui->editFont->setFont(font);
}


void MainWindow::on_btnColor_Write_clicked()
{
    QPalette pale=ui->btnColor_In->palette();
    QColor color= pale.buttonText().color();
    writeByStream(color);
}


void MainWindow::on_btnColor_Read_clicked()
{
    QColor color;
    readByStream(color);
    QPalette pale=ui->editColor->palette();
    pale.setColor(QPalette::Text,color);
    ui->editColor->setPalette(pale);
}


void MainWindow::on_actClearOutput_triggered()
{
    ui->editInt8->clear();
    ui->editUint8->clear();
    ui->editInt16->clear();
    ui->editUint16->clear();
    ui->editInt32->clear();
    ui->editInt64->clear();

    ui->editFloat->clear();
    ui->editFloat16->clear();
    ui->editDouble->clear();

    ui->editString->clear();
    ui->editStringHelloQ->clear();

    QFont font=this->font();
    ui->editFont->setFont(font);

    QPalette plet=this->palette();
    ui->editColor->setPalette(plet);
}


void MainWindow::on_actSaveALL_triggered()
{
    QFile fileDevice(m_filename);
    if(!fileDevice.open(QIODevice::WriteOnly))
        return;

    QDataStream fileStream(&fileDevice);
    fileStream.setVersion(QDataStream::Qt_6_6);
    if(ui->radio_BigEndian->isChecked())
        fileStream.setByteOrder(QDataStream::BigEndian);
    else
        fileStream.setByteOrder(QDataStream::LittleEndian);

    if(ui->radio_Single->isChecked())
        fileStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    else
        fileStream.setFloatingPointPrecision(QDataStream::DoublePrecision);

    qint8 int8_value=ui->spinInt8->value();
    fileStream<<int8_value;

    quint8 uint8_value=ui->spinUint8->value();
    fileStream<<uint8_value;

    qint16 int16_value=ui->spinInt16->value();
    fileStream<<int16_value;

    qint32 int32_value=ui->spinInt32->value();
    fileStream<<int32_value;

    QFont font=ui->btnFont_In->font();
    fileStream<<font;

    QPalette plet=ui->btnColor_In->palette();
    QColor color=plet.buttonText().color();
    fileStream<<color;
    fileDevice.close();
}


void MainWindow::on_actReadALL_triggered()
{
    // 读取全部。
}

