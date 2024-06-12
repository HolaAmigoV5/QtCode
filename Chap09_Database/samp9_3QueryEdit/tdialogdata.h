#ifndef TDIALOGDATA_H
#define TDIALOGDATA_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class TDialogData;
}

class TDialogData : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogData(QWidget *parent = nullptr);
    ~TDialogData();

    QSqlRecord record();
    void setRecord(const QSqlRecord &newRecord);

private slots:
    void on_btnInput_clicked();

    void on_btnClearPhoto_clicked();

private:
    Ui::TDialogData *ui;

    QSqlRecord m_record;

};

#endif // TDIALOGDATA_H
