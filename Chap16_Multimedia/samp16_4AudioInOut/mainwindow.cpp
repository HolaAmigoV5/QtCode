#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


void MainWindow::iniChart()
{
    QChart *chart=new QChart();
    chart->setTitle("音频输入原始信号");
    ui->chartView->setChart(chart);

    lineSeries=new QLineSeries();
    chart->addSeries(lineSeries);
    lineSeries->setUseOpenGL(true);


    QValueAxis *axisX=new QValueAxis;
    axisX->setRange(0,m_maxSize);
    axisX->setLabelFormat("%g");
    axisX->setTitleText("Samples");

    QValueAxis *axisY=new QValueAxis;
    axisY->setRange(0,256);
    axisY->setTitleText("Audio Level");

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    lineSeries->attachAxis(axisX);
    lineSeries->attachAxis(axisY);

    chart->legend()->hide();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    iniChart();

    QAudioDevice device=QMediaDevices::defaultAudioInput();
    if(device.isNull()){
        ui->actStart->setEnabled(false);
        ui->groupBoxDevice->setTitle("音频输入设置（无设备）");
        QMessageBox::information(this,"提示","无音频输入设备");
        return;
    }

    ui->comboDevices->addItem(device.description());

    // 首选音频格式
    QAudioFormat audioFormat=device.preferredFormat();
    ui->comboSampFormat->setCurrentIndex(audioFormat.sampleFormat());
    ui->spinSampRate->setValue(audioFormat.sampleRate());
    int minRate=device.minimumSampleRate();
    int maxRate=device.maximumSampleRate();
    ui->labSampRateRange->setText(QString::asprintf("范围：%d~%d",minRate,maxRate));

    ui->spinChanCount->setValue(audioFormat.channelCount());
    int minChan=device.minimumChannelCount();
    int maxChan=device.maximumChannelCount();
    ui->labChanCountRange->setText(QString::asprintf("范围：%d~%d",minChan,maxChan));

    ui->spinBytesPerSamp->setValue(audioFormat.bytesPerSample());
    ui->spinBytesPerFrame->setValue(audioFormat.bytesPerFrame());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::do_IODevice_update(qint64 blockSize)
{
    float time=audioSource->processedUSecs()/1000; //ms
    QString str=QString::asprintf("已录制时间 =%.1f 秒", time/1000);
    ui->labBufferSize->setText(str);
    ui->labBlockSize->setText(QString("实际数据块字节数=%1").arg(blockSize));
}

void MainWindow::do_sink_stateChanged(QAudio::State state)
{
    if (state == QAudio::IdleState)  //播放结束后的空闲状态
    {
        sinkFileDevice.close();     //关闭文件
        audioSink->stop();          //停止播放
        audioSink->deleteLater();   //在主循环里删除对象
        ui->actPlayFile->setEnabled(true);
        m_isWorking=false;          //表示没有设备在工作了
    }
}

void MainWindow::on_btnGetFile_clicked()
{
    QString curPath=QDir::currentPath();
    QString dlgTitle="选择输出文件";
    QString filter="原始音频数据文件(*.raw);;所有文件(*.*)";
    QString selectedFile=QFileDialog::getSaveFileName(this, dlgTitle,curPath, filter);
    if(!selectedFile.isEmpty()){
        ui->editFileName->setText(selectedFile);
        QFileInfo fileInfo(selectedFile);
        QDir::setCurrent(fileInfo.absolutePath());
    }
}


void MainWindow::on_spinChanCount_valueChanged(int arg1)
{
    ui->spinBytesPerFrame->setValue(arg1*ui->spinBytesPerSamp->value());
    bool canShowWave=(arg1==1)&& (ui->comboSampFormat->currentIndex()==1);
    ui->chkBoxShowWave->setEnabled(canShowWave);

    if(!canShowWave)
        ui->chkBoxShowWave->setChecked(false);
}


void MainWindow::on_comboSampFormat_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
    case 1: // uint8
        ui->spinBytesPerSamp->setValue(1);
        break;
    case 2: // uint16
        ui->spinBytesPerSamp->setValue(2);
        break;
    case 3: // uint32
    case 4: // float
        ui->spinBytesPerSamp->setValue(4);
        break;
    default:
        break;
    }

    int bytes=ui->spinChanCount->value()*ui->spinBytesPerSamp->value();
    ui->spinBytesPerFrame->setValue(bytes);
    bool canShowWave=(index==1)&&(ui->spinChanCount->value()==1);
    ui->chkBoxShowWave->setEnabled(canShowWave);
    if(!canShowWave)
        ui->chkBoxShowWave->setChecked(false);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(m_isWorking){
        QMessageBox::information(this,"提示","正在采集或播放音频，不允许退出");
        event->ignore();
    }
    else
        event->accept();
}

void MainWindow::on_chkBoxSaveToFile_clicked(bool checked)
{
    ui->btnGetFile->setEnabled(checked);
    ui->editFileName->setEnabled(checked);
}


void MainWindow::on_actStart_triggered()
{
    if(ui->comboSampFormat->currentIndex()==0){
        QMessageBox::critical(this, "错误","请设置采样点格式");
        return;
    }

    bool saveToFile=ui->chkBoxSaveToFile->isChecked();
    QString fileName=ui->editFileName->text().trimmed();
    if(saveToFile&&fileName.isEmpty()){
        QMessageBox::critical(this,"错误","请设置保存的文件");
        return;
    }

    // 设置音频格式
    QAudioFormat dagFormat;
    dagFormat.setSampleRate(ui->spinSampRate->value());
    dagFormat.setChannelCount(ui->spinChanCount->value());
    dagFormat.setSampleFormat(QAudioFormat::SampleFormat(ui->comboSampFormat->currentIndex()));

    audioSource=new QAudioSource(dagFormat,this);
    audioSource->setBufferSize(m_maxSize);
    audioSource->setVolume(1);

    myDevice=new TMyDevice(this);
    connect(myDevice,&TMyDevice::updateBlockSize, this, &MainWindow::do_IODevice_update);

    bool showWave=ui->chkBoxShowWave->isChecked();
    myDevice->openDAQ(m_maxSize,showWave,lineSeries,saveToFile,fileName);
    audioSource->start(myDevice);   //以流设备作为参数，开始录入音频输入数据

    m_isWorking=true;
    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);
    ui->actPlayFile->setEnabled(false);
}


void MainWindow::on_actStop_triggered()
{
    audioSource->stop();
    myDevice->closeDAQ();
    delete myDevice;
    delete audioSource;

    m_isWorking=false;
    ui->actStart->setEnabled(true);
    ui->actStop->setEnabled(false);
    ui->actPlayFile->setEnabled(ui->chkBoxSaveToFile->isChecked());
}


void MainWindow::on_actTest_triggered()
{
    QAudioFormat dagFormat;
    dagFormat.setSampleRate(ui->spinSampRate->value());
    dagFormat.setChannelCount(ui->spinChanCount->value());
    dagFormat.setSampleFormat(QAudioFormat::SampleFormat(ui->comboSampFormat->currentIndex()));

    QAudioDevice device=QMediaDevices::defaultAudioInput();
    if(device.isFormatSupported(dagFormat))
        QMessageBox::information(this,"提示","默认设备支持所选格式");
    else
        QMessageBox::information(this,"提示","默认设备不支持所选格式");
}


void MainWindow::on_actPlayFile_triggered()
{
    QString filename=ui->editFileName->text().trimmed();
    if(filename.isEmpty() || !QFile::exists(filename)){
        QMessageBox::critical(this,"错误","打开文件时出现错误，无法播放");
        return;
    }

    sinkFileDevice.setFileName(filename);   //文件IO设备设置文件
    if ( !sinkFileDevice.open(QIODeviceBase::ReadOnly))   //以只读方式打开
    {
        QMessageBox::critical(this,"错误","打开文件时出现错误，无法播放");
        return;
    }

    QAudioFormat format;
    format.setSampleRate(ui->spinSampRate->value());
    format.setChannelCount(ui->spinChanCount->value());
    format.setSampleFormat(QAudioFormat::SampleFormat(ui->comboSampFormat->currentIndex()));

    QAudioDevice audioDevice= QMediaDevices::defaultAudioOutput();  //默认的音频输出设备
    if (!audioDevice.isFormatSupported(format))   //是否支持此音频格式参数
    {
        QMessageBox::critical(this,"错误","播放设备不支持此音频格式设置，无法播放");
        return;
    }

    audioSink =new QAudioSink(format, this);   //创建audioSink
    connect(audioSink, &QAudioSink::stateChanged,this, &MainWindow::do_sink_stateChanged);
    audioSink->start(&sinkFileDevice);    //开始播放


    m_isWorking=true;       //表示有设备在工作，不能关闭窗口
    ui->actPlayFile->setEnabled(false);
}


void MainWindow::on_actPreferFormat_triggered()
{
    QAudioFormat audioFormat =QMediaDevices::defaultAudioInput().preferredFormat();
    ui->spinSampRate->setValue(audioFormat.sampleRate());   //采样频率
    ui->comboSampFormat->setCurrentIndex(audioFormat.sampleFormat());   //采样点格式
    ui->spinChanCount->setValue(audioFormat.channelCount());            //通道个数
    ui->spinBytesPerSamp->setValue(audioFormat.bytesPerSample());       //每个采样点的字节数
    ui->spinBytesPerFrame->setValue(audioFormat.bytesPerFrame());       //每帧字节数
}

