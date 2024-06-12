#ifndef TDIALOGLOCATE_H
#define TDIALOGLOCATE_H

#include <QDialog>

namespace Ui {
class TDialogLocate;
}

class TDialogLocate : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogLocate(QWidget *parent = nullptr);
    ~TDialogLocate();

    void setSpinRange(int rowCount, int colCount);

public slots:
    void setSpinValue(int rowNo, int ColNo);

private slots:
    void on_btnSetText_clicked();

signals:
    void changeCellText(int row, int column, QString &text);
    void changeActionEnable(bool enable);

private:
    Ui::TDialogLocate *ui;

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
};



#endif // TDIALOGLOCATE_H
