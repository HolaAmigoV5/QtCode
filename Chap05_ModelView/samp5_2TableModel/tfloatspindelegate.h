﻿#ifndef TFLOATSPINDELEGATE_H
#define TFLOATSPINDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TFloatSpindelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TFloatSpindelegate(QObject *parent = nullptr);

    // QAbstractItemDelegate interface
public:
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};



#endif // TFLOATSPINDELEGATE_H