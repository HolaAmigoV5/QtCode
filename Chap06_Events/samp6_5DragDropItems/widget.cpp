﻿#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    on_radioListSource_clicked();

    ui->listSource->setDragEnabled(true);
    ui->listSource->setAcceptDrops(true);
    ui->listSource->setDefaultDropAction(Qt::CopyAction);
    ui->listSource->setDragDropMode(QAbstractItemView::DragDrop);


    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setAcceptDrops(true);
    ui->listWidget->setDefaultDropAction(Qt::CopyAction);
    ui->listWidget->setDragDropMode(QAbstractItemView::DragDrop);

    ui->treeWidget->setDragEnabled(true);
    ui->treeWidget->setAcceptDrops(true);
    ui->treeWidget->setDefaultDropAction(Qt::CopyAction);
    ui->treeWidget->setDragDropMode(QAbstractItemView::DragDrop);

    ui->tableWidget->setDragEnabled(true);
    ui->tableWidget->setAcceptDrops(true);
    ui->tableWidget->setDefaultDropAction(Qt::MoveAction);
    ui->tableWidget->setDragDropMode(QAbstractItemView::DragDrop);

    ui->listSource->installEventFilter(this);
    ui->listWidget->installEventFilter(this);
    ui->treeWidget->installEventFilter(this);
    ui->tableWidget->installEventFilter(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::refreshToUI(QGroupBox *curGroupBox)
{
    ui->chkBoxAcceptDrops->setChecked(m_itemView->acceptDrops());
    ui->chkBoxDragEnabled->setChecked(m_itemView->dragEnabled());
    ui->comboMode->setCurrentIndex((int)m_itemView->dragDropMode());
    int index=getDropActionIndex(m_itemView->defaultDropAction());
    ui->comboDefaultAction->setCurrentIndex(index);

    QFont font=ui->groupBox_listSource->font();
    font.setBold(false);
    ui->groupBox_listSource->setFont(font);
    ui->groupBox_listWidget->setFont(font);
    ui->groupBox_treeWidget->setFont(font);
    ui->groupBox_tableWidget->setFont(font);

    font.setBold(true);
    curGroupBox->setFont(font);
}

int Widget::getDropActionIndex(Qt::DropAction actionType)
{
    switch (actionType) {
    case Qt::CopyAction:
        return 0;
    case Qt::MoveAction:
        return 1;
    case Qt::LinkAction:
        return 2;
    case Qt::ActionMask:
        return 3;
    default:
        return 0;
    }
}

Qt::DropAction Widget::getDropActionType(int index)
{
    switch (index) {
    case 0:
        return Qt::CopyAction;
    case 1:
        return Qt::MoveAction;
    case 2:
        return Qt::LinkAction;
    case 3:
        return Qt::ActionMask;
    default:
        return Qt::CopyAction;
    }
}

void Widget::on_radioListWidget_clicked()
{
    m_itemView=ui->listWidget;
    refreshToUI(ui->groupBox_listWidget);
}


void Widget::on_radioTreeWidget_clicked()
{
    m_itemView=ui->treeWidget;
    refreshToUI(ui->groupBox_treeWidget);
}


void Widget::on_radioTableWidget_clicked()
{
    m_itemView=ui->tableWidget;
    refreshToUI(ui->groupBox_tableWidget);
}


void Widget::on_radioListSource_clicked()
{
    m_itemView=ui->listSource;
    refreshToUI(ui->groupBox_listSource);
}


void Widget::on_chkBoxAcceptDrops_clicked(bool checked)
{
    m_itemView->setAcceptDrops(checked);
}


void Widget::on_chkBoxDragEnabled_clicked(bool checked)
{
    m_itemView->setDragEnabled(checked);
}


void Widget::on_comboDefaultAction_currentIndexChanged(int index)
{
    m_itemView->setDragDropMode((QAbstractItemView::DragDropMode)index);
}


void Widget::on_comboMode_currentIndexChanged(int index)
{
    m_itemView->setDefaultDropAction(getDropActionType(index));
}

#include <QKeyEvent>
bool Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()!=QEvent::KeyPress)
        return QWidget::eventFilter(watched,event);

    QKeyEvent *keyEvent=static_cast<QKeyEvent*>(event);
    if(keyEvent->key()!=Qt::Key_Delete)
        return QWidget::eventFilter(watched,event);
    if(watched==ui->listSource){
        delete ui->listSource->takeItem(ui->listSource->currentRow());
    }
    else if(watched==ui->listWidget){
        delete ui->listWidget->takeItem(ui->listWidget->currentRow());
    }
    else if(watched==ui->tableWidget)
         delete ui->tableWidget->takeItem(ui->tableWidget->currentRow()
                                         ,ui->tableWidget->currentColumn());
    else if(watched==ui->treeWidget){
        QTreeWidgetItem *curItem=ui->treeWidget->currentItem();
        if(curItem->parent()!=nullptr)
            curItem->parent()->removeChild(curItem);
        else{
            int index=ui->treeWidget->indexOfTopLevelItem(curItem);
            ui->treeWidget->takeTopLevelItem(index);
        }
        delete curItem;
    }
    return true;
}

