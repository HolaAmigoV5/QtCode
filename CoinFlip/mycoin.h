#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QTimer>

class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);
    MyCoin(QString butImg);

    int posX, posY;
    bool flag;
    void changeFlag();  // 改变标志，执行翻转效果
    QTimer *timer1;
    QTimer *timer2;
    int min=1;  // 最小图片
    int max=8;  // 最大图片

    // 执行动画 标志
    bool isAnimation=false;

    // 重写按下
    void mousePressEvent(QMouseEvent *e);

    bool isWin=false;
private:
    void SetImg(QString imgPath);

signals:

};

#endif // MYCOIN_H
