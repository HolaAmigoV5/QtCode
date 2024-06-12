#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QString>

class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    MyPushButton(QString normalImg, QString pressImg="");

    void zoom1();   // 向下跳跃
    void zoom2();   // 向上跳跃

    QString normalImgPath;  // 默认显示图片路径
    QString pressedImgPath; // 按下后显示图片路径

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

private:
    void SetImg(QString imgPath);

signals:

};

#endif // MYPUSHBUTTON_H
