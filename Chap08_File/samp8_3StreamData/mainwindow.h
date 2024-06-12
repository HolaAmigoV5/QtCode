#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString m_filename;
    template<class T> void writeByStream(T Value);
    template<class T> void readByStream(T &Value);

private slots:
    void on_btnFile_clicked();

    void on_btnColor_In_clicked();

    void on_btnFont_In_clicked();

    void on_btnInt8_Write_clicked();

    void on_btnInt8_Read_clicked();

    void on_btnUint8_Write_clicked();

    void on_btnUint8_Read_clicked();

    void on_btnBool_Write_clicked();

    void on_btnBool_Read_clicked();

    void on_btnDouble_Write_clicked();

    void on_btnDouble_Read_clicked();

    void on_btnFontWrite_clicked();

    void on_btnFont_Read_clicked();

    void on_btnColor_Write_clicked();

    void on_btnColor_Read_clicked();

    void on_actClearOutput_triggered();

    void on_actSaveALL_triggered();

    void on_actReadALL_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
