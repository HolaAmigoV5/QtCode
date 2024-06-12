#ifndef SMALLWIDGET_H
#define SMALLWIDGET_H

#include <QWidget>

namespace Ui {
class smallWidget;
}

class smallWidget : public QWidget
{
    Q_OBJECT

public:
    explicit smallWidget(QWidget *parent = nullptr);
    ~smallWidget();

    // 获取值
    int GetCurrentValue();

    // 设置值
    void SetHalfValue(int num);

private:
    Ui::smallWidget *ui;
};

#endif // SMALLWIDGET_H
