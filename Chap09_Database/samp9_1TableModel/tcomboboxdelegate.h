#ifndef TCOMBOBOXDELEGATE_H
#define TCOMBOBOXDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

class TComboBoxdelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TComboBoxdelegate(QObject *parent=nullptr);

    void SetItems(QStringList items, bool editable);


    // QAbstractItemDelegate interface
public:
    // 创建编辑组件
    virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    // 从数据模型获取数据，显示到代理组件中
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;

    // 将代理组件的数据，保存到数据模型中
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    // 更新代理编辑组件的大小
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
    QStringList m_itemList; // 选择列表
    bool m_editable;        // 是否可编辑

public:

};



#endif // TCOMBOBOXDELEGATE_H
