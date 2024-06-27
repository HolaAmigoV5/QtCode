#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QAudioOutput>


void MainWindow::do_playbackStateChanged(QMediaPlayer::PlaybackState newState)
{
    bool isPlaying=(newState==QMediaPlayer::PlayingState);
    ui->btnPlay->setEnabled(!isPlaying);
    ui->btnPause->setEnabled(isPlaying);
    ui->btnStop->setEnabled(isPlaying);
}

void MainWindow::do_positionChanged(qint64 position)
{
    if(ui->sliderPosition->isSliderDown())
        return;

    ui->sliderPosition->setSliderPosition(position);

    int secs=position/1000;
    int mins=secs/60;
    secs%=60;
    positionTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum(duration);

    int secs=duration/1000;
    int mins=secs/60;
    secs%=60;

    durationTime=QString::asprintf("%d:%d",mins,secs);
    ui->LabRatio->setText(positionTime+"/"+durationTime);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player=new QMediaPlayer(this);
    QAudioOutput *audioOutput=new QAudioOutput(this);
    player->setAudioOutput(audioOutput);
    player->setVideoOutput(ui->videoWidget);
    ui->videoWidget->setMediaPlayer(player);

    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::do_playbackStateChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::do_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnAdd_clicked()
{
    QString curPath=QDir::homePath();
    QString dlgTitle="选择视频文件";
    QString filter="视频文件(*.wmv, *.mp4);;所有文件(*.*)";
    QString aFile=QFileDialog::getOpenFileName(this,dlgTitle,curPath,filter);

    if(aFile.isEmpty())
        return;

    QFileInfo fileInfo(aFile);
    ui->labCurMedia->setText(fileInfo.fileName());
    player->setSource(QUrl::fromLocalFile(aFile));
    player->play();
}


void MainWindow::on_btnPlay_clicked()
{
    player->play();
}


void MainWindow::on_btnPause_clicked()
{
    player->pause();
}


void MainWindow::on_btnStop_clicked()
{
    player->stop();
}


void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    player->audioOutput()->setVolume(value/100.0);
}


void MainWindow::on_btnSound_clicked()
{
    bool mute=player->audioOutput()->isMuted();
    player->audioOutput()->setMuted(!mute);
    if(mute)
        ui->btnSound->setIcon(QIcon(":/images/images/volumn.bmp"));
    else
        ui->btnSound->setIcon(QIcon(":/images/images/mute.bmp"));
}


void MainWindow::on_pushButton_clicked()
{
    ui->videoWidget->setFullScreen(true);
}


void MainWindow::on_sliderPosition_valueChanged(int value)
{
    player->setPosition(value);
}

