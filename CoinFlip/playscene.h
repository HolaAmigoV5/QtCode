#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <mycoin.h>

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelNum);

    int levelIndex; // 记录关卡

    void paintEvent(QPaintEvent *);
    int gameArray[4][4];

    MyCoin *btnCoin[4][4];

    bool isWin;    // 是否胜利

signals:
    void playSceneBack();
};

#endif // PLAYSCENE_H
