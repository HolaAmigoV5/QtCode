﻿#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QAbstractItemView>
#include <QGroupBox>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QAbstractItemView *m_itemView=nullptr;
    void refreshToUI(QGroupBox *curGroupBox);

    int getDropActionIndex(Qt::DropAction actionType);
    Qt::DropAction getDropActionType(int index);

private slots:
    void on_radioListWidget_clicked();

    void on_radioTreeWidget_clicked();

    void on_radioTableWidget_clicked();

    void on_radioListSource_clicked();

    void on_chkBoxAcceptDrops_clicked(bool checked);

    void on_chkBoxDragEnabled_clicked(bool checked);

    void on_comboDefaultAction_currentIndexChanged(int index);

    void on_comboMode_currentIndexChanged(int index);

private:
    Ui::Widget *ui;

    // QObject interface
public:
    virtual bool eventFilter(QObject *watched, QEvent *event) override;
};


#endif // WIDGET_H
