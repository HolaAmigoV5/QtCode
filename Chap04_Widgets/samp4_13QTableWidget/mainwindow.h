#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum FieldColNum{colName=0, colSex, colBirth, colNation, colScore, colIsParty};
    enum CellType{ctName=1000, ctSex, ctBirth, ctNation, ctScore, ctPartyM };

    QLabel *labCellIndex;
    QLabel *labCellType;
    QLabel *labStudId;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnSetHeader_clicked();

    void on_btnSetRows_clicked();

    void on_btnIniData_clicked();

    void on_btnInsertRow_clicked();

    void on_btnAddRow_clicked();

    void on_btnRemoveRow_clicked();

    void on_btnAutoHeight_clicked();

    void on_btnAutoWidth_clicked();

    void on_btnReadText_clicked();

    void on_chkEdiable_clicked(bool checked);

    void on_chkRowColor_clicked(bool checked);

    void on_chkShowHHeader_clicked(bool checked);

    void on_chkShowVHeader_clicked(bool checked);

    void on_rBtnRowSelect_clicked();

    void on_rBtnCellSelect_clicked();

    void on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn);

private:
    Ui::MainWindow *ui;

private:
    void createItemARow(int rowNum, QString name, QString sex, QDate birth, QString nation, int score, bool isParty);
};
#endif // MAINWINDOW_H
