#ifndef TBATTERY_H
#define TBATTERY_H

#include <QWidget>
#include <QtUiPlugin/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT TBattery : public QWidget
{
    Q_OBJECT

public:
    explicit TBattery(QWidget *parent = nullptr);

    void setPowerLevel(int pow);    // 设置当前电量值
    int powerLevel();
    void setWarnLevel(int warn);    // 设置电量低阈值
    int warnLevel();
    QSize sizeHint();               // 重定义的函数，设置组件的合适大小


signals:
    void powerLevelChanged(int);    // 自定义信号
private:
    QColor colorBack=Qt::white;
    QColor colorBorder=Qt::black;
    QColor colorPower=Qt::green;
    QColor colorWarning=Qt::red;
    int m_powerLevel=60;
    int m_warnLevel=20;

protected:
    void paintEvent(QPaintEvent *event);    // 绘制组件并显示效果

};

#endif // TBATTERY_H
