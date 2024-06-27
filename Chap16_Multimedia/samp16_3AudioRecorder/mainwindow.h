#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtMultimedia>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QMediaCaptureSession *session;
    QMediaRecorder *recorder;

private slots:
    void do_recorderStateChanged(QMediaRecorder::RecorderState state);
    void do_durationChanged(qint64 duration);

    void on_btnGetFile_clicked();

    void on_actRecord_triggered();

    void on_actPause_triggered();

    void on_actStop_triggered();

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
