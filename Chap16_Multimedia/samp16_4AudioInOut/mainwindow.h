#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtMultimedia>
#include <QtCharts>

#include <tmydevice.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLineSeries *lineSeries;
    const int m_maxSize=20000;
    bool  m_isWorking=false;    //是否正在采集或播放

    TMyDevice *myDevice;        // 用于显示IODevice
    QAudioSource *audioSource;  // 用于采集原始音频
    QAudioSink *audioSink;      // 用于播放原始音频
    QFile sinkFileDevice;       // 用于audioSink的文件设备

    void iniChart();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void do_IODevice_update(qint64 blockSize);

    void do_sink_stateChanged(QAudio::State state);

    void on_btnGetFile_clicked();

    void on_spinChanCount_valueChanged(int arg1);

    void on_comboSampFormat_currentIndexChanged(int index);

    void on_actStart_triggered();

    void on_actStop_triggered();

    void on_chkBoxSaveToFile_clicked(bool checked);

    void on_actTest_triggered();

    void on_actPlayFile_triggered();

    void on_actPreferFormat_triggered();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};


#endif // MAINWINDOW_H
