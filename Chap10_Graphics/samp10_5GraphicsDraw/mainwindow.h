﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QGraphicsItem>

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

private slots:
    void on_actItem_Rect_triggered();

    void on_actItem_Ecllipse_triggered();

    void on_actItem_Circle_triggered();

    void on_actItem_Triangle_triggered();

    void on_actItem_Polygon_triggered();

    void on_actItem_Line_triggered();

    void on_actItem_Text_triggered();

    // 自定义
    void do_mouseMovePoint(QPoint point);
    void do_mouseClicked(QPoint point);
    void do_mouseDoubleClicked(QPoint point);
    void do_keyPress(QKeyEvent *event);

    void on_actZoomIn_triggered();

    void on_actZoomOut_triggered();

    void on_actRotateLeft_triggered();

    void on_actRotateRight_triggered();

    void on_actRestore_triggered();

    void on_actEdit_Back_triggered();

    void on_actEdit_Front_triggered();

    void on_actGroup_triggered();

    void on_actGroupBreak_triggered();

    void on_actEdit_delete_triggered();

    void on_actHelp_triggered();

private:
    Ui::MainWindow *ui;

private:
    QLabel *labViewCord;
    QLabel *labSceneCord;
    QLabel *labItemCord;
    QLabel *labItemInfo;

    const int ItemId=1;
    const int ItemDescription=2;

    int seqNum=0;   // 图形项的编号
    const quint32 boundValue=100;
    int frontZ=0;
    int backZ=0;

    void setItemProperty(QGraphicsItem *item, QString description);

    template<class T>
    void setBrushColor(T *item);

};
#endif // MAINWINDOW_H