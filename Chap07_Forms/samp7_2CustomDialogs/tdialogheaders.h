#ifndef TDIALOGHEADERS_H
#define TDIALOGHEADERS_H

#include <QDialog>
#include <QStringListModel>
#include <QStringList>

namespace Ui {
class TDialogHeaders;
}

class TDialogHeaders : public QDialog
{
    Q_OBJECT

public:
    explicit TDialogHeaders(QWidget *parent = nullptr);
    ~TDialogHeaders();

    void setHeaderList(QStringList &heads);
    QStringList headerList();

private:
    Ui::TDialogHeaders *ui;
    QStringListModel *m_model;
};

#endif // TDIALOGHEADERS_H
