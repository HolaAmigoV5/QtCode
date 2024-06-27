#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QFileDialog>

void MainWindow::do_recorderStateChanged(QMediaRecorder::RecorderState state)
{
    bool isRecording=(state==QMediaRecorder::RecordingState);
    ui->actRecord->setEnabled(!isRecording);
    ui->actPause->setEnabled(isRecording);
    ui->actStop->setEnabled(isRecording);

    ui->btnGetFile->setEnabled(state==QMediaRecorder::StoppedState);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    ui->labPassTime->setText(QString("已录制%1秒").arg(duration/1000));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    session=new QMediaCaptureSession(this);
    recorder=new QMediaRecorder(this);

    QAudioInput *audioInput=new QAudioInput(this);
    session->setAudioInput(audioInput);
    session->setRecorder(recorder);

    if(QMediaDevices::defaultAudioInput().isNull()){
        ui->actRecord->setEnabled(false);
        ui->groupBoxDevice->setTitle("录音设置（无设备）");
        QMessageBox::information(this,"提示","无音频设备");
        return;
    }

    connect(recorder, &QMediaRecorder::recorderStateChanged,this,&MainWindow::do_recorderStateChanged);
    connect(recorder, &QMediaRecorder::durationChanged,this,&MainWindow::do_durationChanged);

    foreach (const auto &item, QMediaDevices::audioInputs()) {
        ui->comboDevices->addItem(item.description(),QVariant::fromValue(item));
    }

    // 编码格式，文件格式
    QMediaFormat format;
    foreach (const auto &item, format.supportedAudioCodecs(QMediaFormat::Encode)) {
        ui->comboCodec->addItem(QMediaFormat::audioCodecDescription(item),QVariant::fromValue(item));
    }

    foreach (const auto &item, format.supportedFileFormats(QMediaFormat::Encode)) {
        ui->comboFileFormat->addItem(QMediaFormat::fileFormatDescription(item),QVariant::fromValue(item));
    }

    // 采样频率
    int minSampRate=audioInput->device().minimumSampleRate();
    int maxSampRate=audioInput->device().maximumSampleRate();
    ui->comboSampleRate->addItem(QString("Minimum %1").arg(minSampRate), minSampRate);
    ui->comboSampleRate->addItem(QString("Maximum %1").arg(maxSampRate), maxSampRate);
    ui->comboSampleRate->addItem("16000",16000);
    ui->comboSampleRate->addItem("44100",44100);
    ui->comboSampleRate->addItem("48000",48000);
    ui->comboSampleRate->addItem("88200",88200);


    // 通道数
    int minChannel=audioInput->device().minimumChannelCount();
    int maxChannel=audioInput->device().maximumChannelCount();
    ui->comboChannels->addItem(QString("Minimum %1").arg(minChannel), minChannel);
    ui->comboChannels->addItem(QString("Maximum %1").arg(maxChannel), maxChannel);
    ui->comboChannels->addItem("1", 1);
    ui->comboChannels->addItem("2", 2);

    // 品质
    ui->sliderQuality->setRange(0,(int)QImageCapture::VeryHighQuality);
    ui->sliderQuality->setValue((int)QImageCapture::NormalQuality);

    // 固定比特率
    ui->comboBitrate->addItem("32000");
    ui->comboBitrate->addItem("64000");
    ui->comboBitrate->addItem("96000");
    ui->comboBitrate->addItem("128000");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGetFile_clicked()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle="选择输出文件";
    QString filter="所有文件(*.*);;MP3文件(*.mp3);;WMA文件(*.wma);;MP4文件(*.mp4)";
    QString selectedFile=QFileDialog::getSaveFileName(this,dlgTitle,curPath,filter);

    if(selectedFile.isEmpty())
        return;

    ui->editOutputFile->setText(selectedFile);
    QFileInfo fileInfo(selectedFile);

    QDir::setCurrent(fileInfo.absolutePath());

}


void MainWindow::on_actRecord_triggered()
{
    if(recorder->recorderState()==QMediaRecorder::PausedState){
        recorder->record();
        return;
    }

    QString selectedFile=ui->editOutputFile->text().trimmed();
    if(selectedFile.isEmpty())
    {
        QMessageBox::critical(this,"错误","请先设置输出文件！");
        return;
    }

    if(QFile::exists(selectedFile)){
        QFile::remove(selectedFile);
    }

    // 设置输出文件
    recorder->setOutputLocation(QUrl::fromLocalFile(selectedFile));

    // 设置输入设备
    QAudioDevice audioDevice= ui->comboDevices->itemData(ui->comboDevices->currentIndex()).value<QAudioDevice>();
    session->audioInput()->setDevice(audioDevice);

    // 文件格式、编码格式
    QMediaFormat mediaFormat;
    QMediaFormat::FileFormat fileFormat=
        ui->comboFileFormat->itemData(ui->comboFileFormat->currentIndex()).value<QMediaFormat::FileFormat>();
    QMediaFormat::AudioCodec audioCodec=
        ui->comboCodec->itemData(ui->comboCodec->currentIndex()).value<QMediaFormat::AudioCodec>();

    mediaFormat.setFileFormat(fileFormat);
    mediaFormat.setAudioCodec(audioCodec);
    recorder->setMediaFormat(mediaFormat);

    // 设置其他参数
    recorder->setAudioSampleRate(ui->comboSampleRate->itemData(ui->comboSampleRate->currentIndex()).toInt());
    recorder->setAudioChannelCount(ui->comboChannels->itemData(ui->comboChannels->currentIndex()).toInt());


    if(ui->radioQuality->isChecked()){
        recorder->setEncodingMode(QMediaRecorder::ConstantQualityEncoding);
        recorder->setQuality((QMediaRecorder::Quality)ui->sliderQuality->value());
    }
    else{
        recorder->setEncodingMode(QMediaRecorder::ConstantBitRateEncoding);
        recorder->setAudioBitRate(ui->comboBitrate->currentText().toInt());
    }

    recorder->record();
}


void MainWindow::on_actPause_triggered()
{
    recorder->pause();
}


void MainWindow::on_actStop_triggered()
{
    recorder->stop();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(recorder->recorderState()!=QMediaRecorder::StoppedState){
        QMessageBox::information(this,"提示","正在录音，不能退出！");
        event->ignore();
    }
    else
        event->accept();
}

