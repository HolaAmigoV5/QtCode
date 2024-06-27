#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDataVisualization>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QWidget *graphContainer;
    Q3DBars *graph3D;
    QBar3DSeries *series;

    void iniGraph3D();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void do_selectedBarChanged(const QPoint &position);

    void on_actSeries_BaseColor_triggered();

    void on_actRedraw_triggered();

    void on_actBar_ChangeValue_triggered();

    void on_actData_Add_triggered();

    void on_actData_Insert_triggered();

    void on_actData_Delete_triggered();

    void on_comboCamera_currentIndexChanged(int index);

    void on_sliderH_valueChanged(int value);

    void on_sliderV_valueChanged(int value);

    void on_sliderZoom_valueChanged(int value);

    void on_btnResetCamera_pressed();

    void on_btnMoveUp_pressed();

    void on_btnMoveDown_clicked();

    void on_btnMoveLeft_clicked();

    void on_btnMoveRight_clicked();

    void on_cBoxTheme_currentIndexChanged(int index);

    void on_spinFontSize_valueChanged(int arg1);

    void on_cBoxSelectionMode_currentIndexChanged(int index);

    void on_chkBoxBackground_clicked(bool checked);

    void on_chkBoxGrid_clicked(bool checked);

    void on_chkBoxReflection_clicked(bool checked);

    void on_chkBoxReverse_clicked(bool checked);

    void on_chkBoxAxisTitle_clicked(bool checked);

    void on_chkBoxAxisBackground_clicked(bool checked);

    void on_cBoxBarStyle_currentIndexChanged(int index);

    void on_chkBoxSmooth_clicked(bool checked);

    void on_chkBoxItemLabel_clicked(bool checked);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
