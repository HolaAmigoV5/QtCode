#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <tmyvideowidget.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMediaPlayer *player;
    QString durationTime;
    QString positionTime;

private slots:
    void do_playbackStateChanged(QMediaPlayer::PlaybackState newState);
    void do_positionChanged(qint64 position);
    void do_durationChanged(qint64 duration);

    void on_btnAdd_clicked();

    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_btnStop_clicked();

    void on_sliderVolumn_valueChanged(int value);

    void on_btnSound_clicked();

    void on_pushButton_clicked();

    void on_sliderPosition_valueChanged(int value);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
