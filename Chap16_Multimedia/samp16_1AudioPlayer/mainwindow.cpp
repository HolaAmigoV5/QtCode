#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->listWidget->installEventFilter(this);
    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);

    player=new QMediaPlayer(this);
    QAudioOutput *audioOutPut =new QAudioOutput(this);
    player->setAudioOutput(audioOutPut);

    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::do_positionChanged);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::do_durationChanged);
    connect(player, &QMediaPlayer::sourceChanged, this, &MainWindow::do_sourceChanged);
    connect(player, &QMediaPlayer::playbackStateChanged, this, &MainWindow::do_playbackStateChanged);
    connect(player, &QMediaPlayer::metaDataChanged, this, &MainWindow::do_metaDataChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ui->labRatio->setText(positionTime+"/" +durationTime);
}

void MainWindow::do_durationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum(duration);

    int secs=duration/1000;
    int mins=secs/60;
    secs%=60;

    durationTime=QString::asprintf("%d:%d",mins,secs);
    ui->labRatio->setText(positionTime+"/" +durationTime);
}

void MainWindow::do_sourceChanged(const QUrl &media)
{
    ui->labCurMedia->setText(media.fileName());
}

void MainWindow::do_playbackStateChanged(QMediaPlayer::PlaybackState newState)
{
    ui->btnPlay->setEnabled(newState!=QMediaPlayer::PlayingState);
    ui->btnPause->setEnabled(newState==QMediaPlayer::PlayingState);
    ui->btnStop->setEnabled(newState==QMediaPlayer::PlayingState);

    qDebug()<<"PlaybackState:"<<newState;

    if((newState==QMediaPlayer::StoppedState)&&loopPlay){
        int count=ui->listWidget->count();
        int curRow=ui->listWidget->currentRow();
        ++curRow;
        curRow=curRow>=count?0:curRow;
        ui->listWidget->setCurrentRow(curRow);
        player->setSource(ui->listWidget->currentItem()->data(Qt::UserRole).value<QUrl>());
        player->play();
    }
}

void MainWindow::do_metaDataChanged()
{
    QMediaMetaData metaData=player->metaData();
    QVariant metaImg=metaData.value(QMediaMetaData::CoverArtImage);
    if(metaImg.isValid()){
        QImage img=metaImg.value<QImage>();
        QPixmap musicPixmp=QPixmap::fromImage(img);
        if(ui->scrollArea->width()<musicPixmp.width())
            ui->labPic->setPixmap(musicPixmp.scaledToWidth(ui->scrollArea->width()-30));
        else
            ui->labPic->setPixmap(musicPixmp);
    }
    else
        ui->labPic->clear();
}


bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() != QEvent::KeyPress)      //不是KeyPress事件，退出
        return QWidget::eventFilter(watched,event);

    QKeyEvent *keyEvent=static_cast<QKeyEvent *>(event);
    if (keyEvent->key() != Qt::Key_Delete)      //按下的不是Delete键，退出
        return QWidget::eventFilter(watched,event);

    if (watched==ui->listWidget)
    {
        QListWidgetItem *item= ui->listWidget->takeItem(ui->listWidget->currentRow());
        delete  item;
    }
    return true;    //表示事件已经被处理
}

QUrl MainWindow::getUrlFromItem(QListWidgetItem *item)
{
    QVariant itemData= item->data(Qt::UserRole);    //获取用户数据
    QUrl source =itemData.value<QUrl>();    //QVariant转换为QUrl类型
    return source;
}

void MainWindow::on_btnAdd_clicked()
{
    QString curPath=QDir::homePath();   // 获取系统当前目录
    QString dlgTitle="选择音频文件";
    QString filter="音频文件(*.mp3 *.wav *.wma);; 所有文件(*.*)";
    QStringList fileList=QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);
    if(fileList.count()<1)
        return;

    for (int i = 0; i < fileList.size(); ++i) {
        QString aFile=fileList.at(i);
        QFileInfo fileInfo(aFile);
        QListWidgetItem *aItem=new QListWidgetItem(fileInfo.fileName());
        aItem->setIcon(QIcon(":/images/images/musicFile.png"));
        aItem->setData(Qt::UserRole, QUrl::fromLocalFile(aFile));
        ui->listWidget->addItem(aItem);
    }

    if(player->playbackState()!=QMediaPlayer::PlayingState){
        ui->listWidget->setCurrentRow(0);
        QUrl source=getUrlFromItem(ui->listWidget->currentItem());
        player->setSource(source);
    }

    player->play();
}


void MainWindow::on_btnRemove_clicked()
{
    int index=ui->listWidget->currentRow();
    if(index>=0){
        QListWidgetItem *item=ui->listWidget->takeItem(index);
        delete item;
    }
}


void MainWindow::on_btnClear_clicked()
{
    loopPlay=false;
    ui->listWidget->clear();
    player->stop();
}


void MainWindow::on_btnPlay_clicked()
{
    if(player->playbackState()!=QMediaPlayer::PausedState){
        if(ui->listWidget->currentRow()<0)
            ui->listWidget->setCurrentRow(0);
        player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    }

    player->play();
    loopPlay=ui->btnLoop->isChecked();
}


void MainWindow::on_btnPause_clicked()
{
    player->pause();
}


void MainWindow::on_btnStop_clicked()
{
    loopPlay=false;
    player->stop();
}

// 上一曲
void MainWindow::on_btnPrevious_clicked()
{
    int curRow=ui->listWidget->currentRow();
    curRow--;
    curRow=curRow<0?0:curRow;
    ui->listWidget->setCurrentRow(curRow);

    loopPlay=false;
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    player->play();
    loopPlay=ui->btnLoop->isChecked();
}

// 下一曲
void MainWindow::on_btnNext_clicked()
{
    int curRow=ui->listWidget->currentRow();
    int count=ui->listWidget->count();
    curRow++;
    curRow=curRow>=count?count-1:curRow;
    ui->listWidget->setCurrentRow(curRow);

    loopPlay=false;
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    player->play();
    loopPlay=ui->btnLoop->isChecked();
}


void MainWindow::on_btnLoop_clicked(bool checked)
{
    loopPlay=checked;
}

// 倍速
void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    player->setPlaybackRate(arg1);
}


void MainWindow::on_sliderVolumn_valueChanged(int value)
{
    player->audioOutput()->setVolume(value/100.0);      // 0~1之间
}


void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    loopPlay=false;
    player->setSource(getUrlFromItem(ui->listWidget->currentItem()));
    player->play();
    loopPlay=ui->btnLoop->isChecked();
}


void MainWindow::on_sliderPosition_valueChanged(int value)
{
    player->setPosition(value);
}

