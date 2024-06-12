#include "tcomboboxdelegate.h"
#include <QComboBox>

TComboBoxDelegate::TComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate{parent}
{}

void TComboBoxDelegate::setItems(QStringList items, bool editable)
{
    m_itemList=items;
    m_editable=editable;
}


QWidget *TComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option); Q_UNUSED(index);

    QComboBox *editor=new QComboBox(parent);
    editor->setEditable(m_editable);
    for(const auto &item:m_itemList){
        editor->addItem(item);
    }
    return editor;
}

void TComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QComboBox *comboBox=dynamic_cast<QComboBox*>(editor);
    comboBox->setCurrentText(index.model()->data(index).toString());
}

void TComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QComboBox *comboBox=dynamic_cast<QComboBox*>(editor);
    model->setData(index,comboBox->currentText());
}

void TComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
