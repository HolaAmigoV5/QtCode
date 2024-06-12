#include "tfloatspindelegate.h"
#include <QDoubleSpinBox>

TFloatSpindelegate::TFloatSpindelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}


QWidget *TFloatSpindelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);Q_UNUSED(index);

    QDoubleSpinBox *editor=new QDoubleSpinBox(parent);
    editor->setFrame(false);
    editor->setMinimum(0);
    editor->setMaximum(50000);
    return editor;
}

void TFloatSpindelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox=dynamic_cast<QDoubleSpinBox*>(editor);
    double value=index.model()->data(index,Qt::EditRole).toDouble();
    spinBox->setValue(value);
}

void TFloatSpindelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QDoubleSpinBox *spinBox=dynamic_cast<QDoubleSpinBox*>(editor);
    double value=spinBox->value();
    model->setData(index,value);
}

void TFloatSpindelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
