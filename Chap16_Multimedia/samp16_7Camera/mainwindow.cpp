#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

void MainWindow::showCameraDeviceInfo(QCameraDevice *device)
{
    ui->comboCam_Position->setCurrentIndex(device->position());
    ui->comboCam_PhotoRes->clear();
    ui->comboImage_Resolution->clear();
    for(const auto &item:device->photoResolutions()){
        QString str=QString::asprintf("%d X %d", item.width(),item.height());
        ui->comboCam_PhotoRes->addItem(str);
        ui->comboImage_Resolution->addItem(str,item);
    }

    ui->comboCam_VideoRes->clear();
    ui->comboCam_FrameRate->clear();
    ui->comboVideo_Resolution->clear();

    for(const auto&item:device->videoFormats()){
        QSize size=item.resolution();
        QString str=QString::asprintf("%d X %d", size.width(),size.height());

        ui->comboCam_VideoRes->addItem(str);
        ui->comboVideo_Resolution->addItem(str,size);

        str=QString::asprintf("%.1f ~ %.1f", item.minFrameRate(), item.maxFrameRate());
        ui->comboCam_FrameRate->addItem(str);
    }
}

void MainWindow::showCameraSupportFeatures(QCamera *aCamera)
{
    QCamera::Features features=aCamera->supportedFeatures();
    bool supported=features.testFlag(QCamera::Feature::ColorTemperature);
    ui->chkBoxCam_Color->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::ExposureCompensation);
    ui->chkBoxCam_Exposure->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::IsoSensitivity);
    ui->chkBoxCam_Iso->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::ManualExposureTime);
    ui->chkBoxCam_Manual->setChecked(supported);

    supported=features.testFlag(QCamera::Feature::CustomFocusPoint);
    ui->chkBoxCam_Custom->setChecked(supported);

    ui->chkBoxCam_Focus->setChecked(features.testFlag(QCamera::Feature::FocusDistance));
}

void MainWindow::do_readyForCaptureChanged(bool ready)
{
    ui->actCapture->setEnabled(ready);
}

void MainWindow::do_imageCaptured(int id, const QImage &preview)
{
    Q_UNUSED(id);
    QString str=QString::asprintf("实际图片分辨率= %d X %d",preview.width(),preview.height());
    labFormatInfo->setText(str);
    QImage scaltedImage=preview.scaledToWidth(ui->scrollArea->width()-30);
    ui->labImage->setPixmap(QPixmap::fromImage(scaltedImage));

    if(!ui->chkBox_SaveToFile->isChecked())
        labInfo->setText("图片未保存为文件");
}

void MainWindow::do_imageSaved(int id, const QString &fileName)
{
    Q_UNUSED(id);
    labInfo->setText("图片保存为文件："+fileName);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    labDuration->setText(QString::asprintf("录制时间：%.1f秒",duration/1000.0));
}

void MainWindow::do_recorderStateChanged(QMediaRecorder::RecorderState state)
{
    ui->actVideoRecord->setEnabled(state==QMediaRecorder::StoppedState);
    ui->actVideoStop->setEnabled(state==QMediaRecorder::RecordingState);
}

void MainWindow::do_errorOccurred(QMediaRecorder::Error error, const QString &errorString)
{
    Q_UNUSED(error);
    labInfo->setText(errorString);
}

void MainWindow::do_activeChanged(bool active)
{
    m_isWorking=true;
    ui->actStartCamera->setEnabled(!active);
    ui->actStopCamera->setEnabled(active);
    ui->actVideoRecord->setEnabled(active);
    ui->comboCam_List->setEnabled(!active);
}

void MainWindow::do_currentIndexChanged(int index)
{
    QCameraDevice device= ui->comboCam_List->itemData(index).value<QCameraDevice>();
    camera->setCameraDevice(device);

    // 显示摄像头信息
    showCameraDeviceInfo(&device);
    showCameraSupportFeatures(camera);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labDuration=new QLabel("录制时间");
    labDuration->setMinimumWidth(120);
    ui->statusBar->addWidget(labDuration);

    labFormatInfo=new QLabel("图片分辨率");
    labFormatInfo->setMinimumWidth(150);
    ui->statusBar->addWidget(labFormatInfo);

    labInfo=new QLabel("信息");
    ui->statusBar->addPermanentWidget(labInfo);
    ui->statusBar->addWidget(labInfo);

    // 发现摄像头
    QCameraDevice defaultCameraDevice=QMediaDevices::defaultVideoInput();
    if(defaultCameraDevice.isNull()){
        QMessageBox::critical(this, "警告","没有检测到摄像头");
        return;
    }

    ui->actStartCamera->setEnabled(true);

    int index=0, i=0;
    for(const auto& device:QMediaDevices::videoInputs()){
        if(device.id()==defaultCameraDevice.id()){
            ui->comboCam_List->addItem(device.description()+" （默认）", QVariant::fromValue(device));
            index=i;
        }
        else
             ui->comboCam_List->addItem(device.description(), QVariant::fromValue(device));

        ++i;
    }

    ui->comboCam_List->setCurrentIndex(index);

    // QMediaCaptureSession
    session=new QMediaCaptureSession(this);
    session->setVideoOutput(ui->videoPreview);
    QAudioInput *audioInput=new QAudioInput(this);
    audioInput->setDevice(QMediaDevices::defaultAudioInput());
    session->setAudioInput(audioInput);

    camera=new QCamera(this);
    camera->setCameraDevice(defaultCameraDevice);
    session->setCamera(camera);

    connect(camera, &QCamera::activeChanged,this, &MainWindow::do_activeChanged);
    connect(ui->comboCam_List, &QComboBox::currentIndexChanged,this, &MainWindow::do_currentIndexChanged);

    do_currentIndexChanged(ui->comboCam_List->currentIndex());

    // QImageCapture
    imageCapture=new QImageCapture(this);
    imageCapture->setQuality(QImageCapture::VeryHighQuality);
    session->setImageCapture(imageCapture);
    connect(imageCapture, &QImageCapture::readyForCaptureChanged, this, &MainWindow::do_readyForCaptureChanged);
    connect(imageCapture, &QImageCapture::imageCaptured,this,&MainWindow::do_imageCaptured);
    connect(imageCapture, &QImageCapture::imageSaved,this,&MainWindow::do_imageSaved);

    // 快门声音
    soundEffect=new QSoundEffect(this);
    soundEffect->setSource(QUrl::fromLocalFile(":/sound/images/shutter.wav"));

    // 录像
    recorder=new QMediaRecorder(this);
    session->setRecorder(recorder);
    connect(recorder, &QMediaRecorder::durationChanged, this, &MainWindow::do_durationChanged);
    connect(recorder, &QMediaRecorder::recorderStateChanged, this, &MainWindow::do_recorderStateChanged);
    connect(recorder,&QMediaRecorder::errorOccurred,this, &MainWindow::do_errorOccurred);

    // 视频编码和文件格式
    QMediaFormat format;
    for(const auto& item:format.supportedVideoCodecs(QMediaFormat::Encode)){
        ui->comboVideo_Codec->addItem(QMediaFormat::videoCodecDescription(item),QVariant::fromValue(item));
    }

    for(const auto& item:format.supportedFileFormats(QMediaFormat::Encode)){
        ui->comboVideo_FileFormat->addItem(QMediaFormat::fileFormatDescription(item),QVariant::fromValue(item));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actStartCamera_triggered()
{
    camera->start();
}


void MainWindow::on_actStopCamera_triggered()
{
    if(recorder->recorderState()==QMediaRecorder::RecordingState)
        recorder->stop();
    camera->stop();
}


void MainWindow::on_actCapture_triggered()
{
    ui->tabWidget->setCurrentIndex(0);
    imageCapture->setQuality((QImageCapture::Quality)ui->comboImage_Quality->currentIndex());
    int index=ui->comboImage_Resolution->currentIndex();
    QVariant var=ui->comboImage_Resolution->itemData(index);
    imageCapture->setResolution(var.toSize());

    if(ui->chkBox_SaveToFile->isChecked())
        imageCapture->captureToFile();
    else
        imageCapture->capture();

    if(ui->chkBox_Sound->isChecked())
        soundEffect->play();
}

void MainWindow::on_actVideoRecord_triggered()
{
    QString str=ui->editVideo_OutputFile->text().trimmed();
    if(str.isEmpty()){
        QMessageBox::critical(this,"错误","请先设置录像输出文件");
        return;
    }

    if(QFile::exists(str)){
        if(!QFile::remove(str)){
            QMessageBox::critical(this,"错误","文件正在使用，无法删除！");
            return;
        }
    }

    recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);
    int index=ui->comboVideo_Quality->currentIndex();
    recorder->setQuality((QMediaRecorder::Quality)index);

    // 设置格式
    QMediaFormat mediaFormat;
    QVariant var=ui->comboVideo_Codec->itemData(ui->comboVideo_Codec->currentIndex());
    mediaFormat.setVideoCodec(var.value<QMediaFormat::VideoCodec>());
    mediaFormat.setFileFormat(ui->comboVideo_FileFormat->itemData(ui->comboVideo_FileFormat->currentIndex()).value<QMediaFormat::FileFormat>());
    recorder->setMediaFormat(mediaFormat);

    // 分辨率
    recorder->setVideoResolution(ui->comboVideo_Resolution->itemData(ui->comboVideo_Resolution->currentIndex()).toSize());

    labInfo->clear();
    recorder->setOutputLocation(QUrl::fromLocalFile(str));
    recorder->record();
}

void MainWindow::on_actVideoStop_triggered()
{
    recorder->stop();
}


void MainWindow::on_btnVideoFile_clicked()
{
    QString curPath=QDir::homePath();
    QString dlgTitle="选择保存文件";
    QString filter="MP4视频文件(*.MP4);; WMV视频文件(*.WMV);;所有文件(*.*)";
    QString selectedFile=QFileDialog::getSaveFileName(this, dlgTitle, curPath, filter);
    if(!selectedFile.isEmpty())
        ui->editVideo_OutputFile->setText(selectedFile);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_isWorking){
        if(recorder->recorderState()==QMediaRecorder::RecordingState)
            recorder->stop();
        camera->stop();
    }

    event->accept();
}

