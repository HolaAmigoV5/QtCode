#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);


    // 捕捉事件，鼠标进入事件
    //void enterEvent(QEvent *event);
    //void leaveEvent(QEvent *event);

    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);

    bool event(QEvent *e);

signals:

};

#endif // MYLABEL_H
