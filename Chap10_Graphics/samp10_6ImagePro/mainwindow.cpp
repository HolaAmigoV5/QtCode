#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextBlock>
#include <QPrintDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actFile_Open_triggered()
{
    m_filename=QFileDialog::getOpenFileName(this,"打开图片",QDir::currentPath(),"图片文件(*.bmp *.jpg *.png);;BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png)");
    if(m_filename.isEmpty())
        return;

    ui->statusbar->showMessage(m_filename);
    m_image.load(m_filename);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    ui->tabWidget->setCurrentIndex(0);

    // 显示图片属性
    showImageFeatures();

    // 设置工具栏action的状态
    ui->frameLeft->setEnabled(true);
    ui->actFile_SaveAs->setEnabled(true);

    ui->actImg_ZoomIn->setEnabled(true);
    ui->actImg_ZoomOut->setEnabled(true);
    ui->actImg_FilpLR->setEnabled(true);
    ui->actImg_FlipUD->setEnabled(true);
    ui->actImg_RotateLeft->setEnabled(true);
    ui->actImg_RotateRight->setEnabled(true);
    ui->actFile_Print->setEnabled(true);
    ui->actFile_Preview->setEnabled(true);
}

void MainWindow::showImageFeatures(bool formatChanged)
{
    if(formatChanged){
        QImage::Format fmt=m_image.format();
        if(fmt==QImage::Format_RGB32)
            ui->editImg_Format->setText("32-bit RGB(0xffRRGGBB)");
        else if(fmt==QImage::Format_RGB16)
            ui->editImg_Format->setText("16-bit RGB565");
        else if(fmt==QImage::Format_RGB888)
            ui->editImg_Format->setText("24-bit RGB888");
        else if(fmt==QImage::Format_Grayscale8)
            ui->editImg_Format->setText("8-bit grayscale");
        else if(fmt==QImage::Format_Grayscale16)
            ui->editImg_Format->setText("16-bit grayscale");
        else if(fmt==QImage::Format_ARGB32)
            ui->editImg_Format->setText("32-bit ARGB(0xAARRGGBB)");
        else if(fmt==QImage::Format_Indexed8)
            ui->editImg_Format->setText("8-bit indexes into a colormap");
        else
            ui->editImg_Format->setText(QString("Format = %1, 其他格式").arg(fmt));

        ui->editImg_Depth->setText(QString("%1 bits/pixel").arg(m_image.depth()));
        ui->editImg_BitPlane->setText(QString("%1 bits").arg(m_image.bitPlaneCount()));

        ui->chkBox_Alpha->setChecked(m_image.hasAlphaChannel());
        ui->chkBox_GrayScale->setChecked(m_image.isGrayscale());
    }

    // 大小相关
    ui->editImg_Height->setText(QString("%1 像素").arg(m_image.height()));
    ui->editImg_Width->setText(QString("%1 像素").arg(m_image.width()));

    qsizetype sz=m_image.sizeInBytes();
    if(sz<1024*9)
        ui->editImg_SizeByte->setText(QString("%1 Bytes").arg(sz));
    else
        ui->editImg_SizeByte->setText(QString("%1 KB").arg(sz/1024));

    QString dpi=QString::asprintf("DPI_X=%.0f, DPI_Y=%.0f",m_image.dotsPerMeterX()*0.0254, m_image.dotsPerMeterY()*0.0254);
    ui->editImg_DPM->setText(dpi);
}

void MainWindow::imageModified(bool modified)
{
    ui->actFile_Reload->setEnabled(modified);
    ui->actFile_Save->setEnabled(modified);
}

void MainWindow::printImage(QPainter *painter, QPrinter *printer)
{
    QMargins margin(20,40,20,40);
    QRectF pageRect= printer->pageRect(QPrinter::DevicePixel);
    int pageW=pageRect.width();
    int pageH=pageRect.height();

    const int lineInc=20;       // 一行所占的像素
    int curX=margin.left();
    int curY=margin.top();
    painter->drawText(curX,curY,m_filename);
    curY+=lineInc;      // 移动到下一行

    painter->drawText(curX,curY,QString("Page width =%1 像素").arg(pageW));
    painter->drawText(250,curY,QString("Image width =%1 像素").arg(m_image.width()));
    curY+=lineInc;      // 移动到下一行

    painter->drawText(curX,curY,QString("Page height =%1 像素").arg(pageH));
    painter->drawText(250,curY,QString("Image height =%1 像素").arg(m_image.height()));
    curY+=lineInc;      // 移动到下一行

    int spaceH=pageH-curY;  //剩余的高度
    if((pageW>m_image.width())&&(spaceH>m_image.height())){
        curX=(pageW-m_image.width())/2;
        painter->drawImage(curX,curY,m_image);  // 居中打印
        return;
    }

    // 需要缩放
    QImage newImg;
    if(m_image.height()>spaceH)
        newImg=m_image.scaledToHeight(spaceH);
    else
        newImg=m_image.scaledToWidth(pageW);

    curX=(pageW-newImg.width())/2;
    painter->drawImage(curX,curY,newImg);  // 居中打印
    return;
}

void MainWindow::printRGB565Data(QPainter *painter, QPrinter *printer)
{
    QMargins margin(20,40,20,40);
    QRectF pageRect= printer->pageRect(QPrinter::DevicePixel);
    int pageW=pageRect.width();
    int pageH=pageRect.height();

    const int lineInc=25;       // 一行所占的像素
    int curX=margin.left();
    int curY=margin.top();

    painter->setFont(ui->plainTextEdit->font());


    int pageNum=1;
    painter->drawLine(margin.left(),pageH-margin.bottom()+1,pageW-margin.right(),pageH-margin.bottom()+1);
    painter->drawText(pageW-5*margin.right(), pageH-margin.bottom()+20,QString("第%1页").arg(pageNum));

    QTextDocument *doc=ui->plainTextEdit->document();
    int cnt=doc->blockCount();
    for (int i = 0; i < cnt; ++i) {
        painter->drawText(curX,curY,doc->findBlockByNumber(i).text());
        curY+=lineInc;

        if(curY>=(pageH-margin.bottom())){
            printer->newPage();
            curY=margin.top();
            pageNum++;
            painter->drawLine(margin.left(),pageH-margin.bottom()+1,pageW-margin.right(),pageH-margin.bottom()+1);
            painter->drawText(pageW-5*margin.right(), pageH-margin.bottom()+20,QString("第%1页").arg(pageNum));
        }
    }
}


void MainWindow::on_actImg_ZoomIn_triggered()
{
    m_image=m_image.scaled(m_image.width()*1.1, m_image.height()*1.1);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    showImageFeatures(false);
    imageModified(true);
}


void MainWindow::on_actImg_ZoomOut_triggered()
{
    m_image=m_image.scaled(m_image.width()*0.9, m_image.height()*0.9);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    showImageFeatures(false);
    imageModified(true);
}


void MainWindow::on_actImg_RotateLeft_triggered()
{
    QTransform matrix;
    matrix.reset();
    matrix.rotate(-90);
    m_image=m_image.transformed(matrix);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    showImageFeatures(false);
    imageModified(true);
}


void MainWindow::on_actImg_RotateRight_triggered()
{
    QTransform matrix;
    matrix.reset();
    matrix.rotate(90);
    m_image=m_image.transformed(matrix);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    showImageFeatures(false);
    imageModified(true);
}

// 上下翻转
void MainWindow::on_actImg_FlipUD_triggered()
{
    m_image.mirror();
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    imageModified(true);
}

// 左右翻转
void MainWindow::on_actImg_FilpLR_triggered()
{
    m_image.mirror(true,false);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    imageModified(true);
}

// 重新载入
void MainWindow::on_actFile_Reload_triggered()
{
    m_image.load(m_filename);
    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    ui->tabWidget->setCurrentIndex(0);
    showImageFeatures(false);
    imageModified(true);
}


void MainWindow::on_actFile_Save_triggered()
{
    m_image.save(m_filename);
    imageModified(false);
}

// 另存
void MainWindow::on_actFile_SaveAs_triggered()
{
    QString fileName=QFileDialog::getSaveFileName(this,"保存文件",m_filename,"图片文件(*.bmp *.jpg *.png);;BMP文件(*.bmp);;JPG文件(*.jpg);;PNG文件(*.png)");
    if(fileName.isEmpty())
        return;

    m_filename=fileName;
    m_image.save(m_filename);
    imageModified(false);
    ui->statusbar->showMessage(m_filename);
}

// 图片格式转换
void MainWindow::on_btnFormatConvert_clicked()
{
    int index=ui->comboFormat->currentIndex();
    if(index==0)
        m_image=m_image.convertedTo(QImage::Format_RGB16);
    else if(index==1)
        m_image=m_image.convertedTo(QImage::Format_RGB888);
    else if(index==2)
        m_image=m_image.convertedTo(QImage::Format_RGB32);
    else if(index==3)
        m_image=m_image.convertedTo(QImage::Format_Grayscale8);
    else if(index==4)
        m_image=m_image.convertedTo(QImage::Format_Grayscale16);
    else if(index==5)
        m_image=m_image.convertedTo(QImage::Format_Indexed8);
    else
        return;

    ui->labPic->setPixmap(QPixmap::fromImage(m_image));
    ui->tabWidget->setCurrentIndex(0);
    showImageFeatures(true);
    imageModified(true);
}


void MainWindow::on_btnGetRGB565_clicked()
{
    // 一个像素16位，一个字节8位
    ui->plainTextEdit->clear();
    int total =m_image.width()*m_image.height()*2;
    QFileInfo fileinfo(m_filename);
    QString aline=QString("const unsigned char RGB565_%1[%2]={").arg(fileinfo.baseName()).arg(total);
    ui->plainTextEdit->appendPlainText(aline);
    aline="";

    // 对像素的处理
    for (int y = 0; y < m_image.height(); ++y) {
        for(int x=0; x<m_image.width();++x){
            QRgb rgb=m_image.pixel(x,y);    // 0xAARRGGBB
            quint32 redtmp32=rgb & 0x00F80000; // red 1111 1000
            quint16 rgb565=redtmp32>>8;        // 0xF800, red5
            quint32 gtmp32=rgb&0x0000FC00;           // green 1111 1100
            rgb565=rgb565|(gtmp32>>5);       // red5green6
            quint32 btmp32=rgb&0x000000F8;           // blue 1111 1000
            rgb565=rgb565|(btmp32>>3);       // red5green6blue5

            quint8 byteLSB=rgb565&0x00FF;
            quint8 byteMSB=rgb565>>8;

            if(ui->radioLSB->isChecked()){
                aline+=QString("0x%1, 0x%2, ").arg(byteLSB,2,16,QChar('0')).arg(byteMSB,2,16,QChar('0'));
            }
            else{
                aline+=QString("0x%1, 0x%2, ").arg(byteMSB,2,16,QChar('0')).arg(byteLSB,2,16,QChar('0'));
            }

            static int count=0;
            ++count;
            if(count==8){
                ui->plainTextEdit->appendPlainText(aline);
                count=0;
                aline="";
            }
        }
    }

    ui->plainTextEdit->appendPlainText("}");
    ui->tabWidget->setCurrentIndex(1);
    ui->btnSaveDataFile->setEnabled(true);

    QMessageBox::information(this,"提示","RGB565生成完毕");
}

// 保存数据
void MainWindow::on_btnSaveDataFile_clicked()
{
    QFileInfo fileInfo(m_filename);
    QString newFile=fileInfo.baseName()+".h";
    QString fileName=QFileDialog::getSaveFileName(this,"保存文件",newFile,"头文件(*.h);;C语言文件(*.c);;文本文件(*.txt)");
    if(fileName.isEmpty())
        return;
    QFile aFile(fileName);
    if(aFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        QString str=ui->plainTextEdit->toPlainText();
        aFile.write(str.toUtf8(),str.toUtf8().length());
        aFile.close();
    }
}

// 打印
void MainWindow::on_actFile_Print_triggered()
{
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec()==QDialog::Accepted){
        QPainter painter(&printer);
        if(ui->tabWidget->currentIndex()==0)
            printImage(&painter, &printer);
        else
            printRGB565Data(&painter, &printer);
    }
}

// 打印预览
#include <QPrintPreviewDialog>
void MainWindow::on_actFile_Preview_triggered()
{
    QPrintPreviewDialog previewDlg(this);
    if(ui->tabWidget->currentIndex()==0)
        connect(&previewDlg, &QPrintPreviewDialog::paintRequested,this, &MainWindow::do_paintRequestImage);
    else
        connect(&previewDlg, &QPrintPreviewDialog::paintRequested,this, &MainWindow::do_paintRequestText);
    previewDlg.exec();
}

void MainWindow::do_paintRequestImage(QPrinter *printer)
{
    QPainter painter(printer);
    printImage(&painter,printer);
}

void MainWindow::do_paintRequestText(QPrinter *printer)
{
    QPainter painter(printer);
    printRGB565Data(&painter,printer);
}

