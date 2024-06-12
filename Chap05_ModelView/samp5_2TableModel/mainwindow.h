#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QStandardItemModel>
#include <QItemSelectionModel>

#include "tcomboboxdelegate.h"
#include "tfloatspindelegate.h"
#include "tspinboxdelegate.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QLabel *labCurFile;
    QLabel *labCellPos;
    QLabel *labCellText;

    const int fixedColumnCount=6;

    QStandardItemModel *m_model;
    QItemSelectionModel *m_selectionModel;

    void iniModelData(QStringList &aFileContent);

    TSpinBoxDelegate *m_SpinDelegate;
    TFloatSpindelegate *m_FloatSpinDelegate;
    TComboBoxDelegate *m_comboBoxDelegate;

private slots:
    void do_currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_actDataPreView_triggered();

    void on_actOpenFile_triggered();

    void on_actAppend_triggered();

    void on_actInsert_triggered();

    void on_actDelete_triggered();

    void on_actAlignLeft_triggered();

    void on_actAlignCenter_triggered();

    void on_actAlignRight_triggered();

    void on_actFontBold_triggered(bool checked);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
