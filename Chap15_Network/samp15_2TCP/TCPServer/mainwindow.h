#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTcpServer>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket=nullptr;

    QLabel *labListen;  // 监听状态
    QLabel *labSocketState;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actStart_triggered();

    void do_newConnection();
    void do_connected();

    void on_actStop_triggered();

    void on_actClear_triggered();

    void on_btnSend_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
