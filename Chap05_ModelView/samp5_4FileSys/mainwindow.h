﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileSystemModel>

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
    void on_actSetRoot_triggered();

    void on_radioShowAll_clicked();

    void on_radioShowOnlyDir_clicked();

    void on_chkBoxEnableFilter_clicked(bool checked);

    void on_btnApplyFilters_clicked();

    void on_treeView_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

private:
    QFileSystemModel *m_model;

};
#endif // MAINWINDOW_H
