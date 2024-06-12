#include "playscene.h"
#include <QMenuBar>
#include <QMenu>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <mypushbutton.h>
#include <mycoin.h>
#include<QPropertyAnimation>
#include <QSound>
#include "dataconfig.h"

PlayScene::PlayScene(int levelNum) :levelIndex(levelNum)
{
    // 初始化游戏场景
    this->setFixedSize(520,788);    // 设置固定大小
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("伍哥赏金币");

    // 创建菜单栏
    QMenuBar *mBar=menuBar();
    setMenuBar(mBar);

    // 创建开始菜单
    QMenu *startMenu= mBar->addMenu("开始");
    QAction *quitAction= startMenu->addAction("退出"); // 创建退出菜单项
    connect(quitAction, &QAction::triggered,this,[=](){
        this->close();
    });


    // 添加音效
    QSound *conFlip=new QSound(":/res/ConFlipSound.wav", this);
    QSound *levelWin=new QSound(":/res/LevelWinSound.wav", this);

    // 返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width()*1.2,this->height()-backBtn->height()*1.2);
    connect(backBtn, &QPushButton::clicked,this,[=](){
        // 延时发送信号
        QTimer::singleShot(500, this, [=](){
            // 发送返回信号
            emit this-> playSceneBack();
        });
    });

    // 显示关卡信息
    QLabel *label=new QLabel(this);
    label->setText(QString("Leavel: %1").arg(QString::number(levelIndex)));
    label->setFont(QFont("华文新魏",20,1));
    label->setGeometry(QRect(30, this->height()-50, 200,50));

    // 初始化关卡数据
    dataConfig config;
    for (int i=0; i<4 ;i++ ) {
        for(int j=0; j<4; j++){
            gameArray[i][j]=config.mData[this->levelIndex][i][j];
        }
    }

    // 创建胜利图片
    QLabel *winLabel=new QLabel;
    // 创建QPixmap对象
    QPixmap pix;

    // 加载图片
    pix.load(":/res/LevelCompletedDialogBg.png");
    winLabel->setGeometry(0,0,pix.width(),pix.height());
    winLabel->setPixmap(pix);
    winLabel->setParent(this);
    winLabel->move((this->width()-pix.width())*0.5, -pix.height());

    // 创建金币背景图片
    for (int i=0; i<4 ;i++ ) {
        for(int j=0; j<4; j++){
            // 绘制背景图片
            QPixmap pix=QPixmap(":/res/BoardNode.png");
            QLabel *label=new QLabel(this);
            label->setGeometry(0,0,pix.width(),pix.height());
            label->setPixmap(pix);
            label->move(150+i*50, 300+j*50);


            // 创建金币
            QString str;
            if(this->gameArray[i][j]==1){
                str=":/res/Coin0001.png";
            }
            else
                str=":/res/Coin0008.png";

            MyCoin *coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(152+i*50, 303+j*50);
            coin->posX=i;
            coin->posY=j;
            coin->flag=gameArray[i][j];
            btnCoin[i][j]=coin;

            // 点击金币后，进行翻转。
            connect(coin, &QPushButton::clicked, this, [=](){
                conFlip->play();

                coin->changeFlag();
                gameArray[i][j]=!gameArray[i][j];

                // 周围金币翻转,延时翻转
                QTimer::singleShot(300,this, [=](){
                    if(coin->posX+1<4){
                        // 翻转右侧金币
                        btnCoin[coin->posX+1][coin->posY]->changeFlag();
                        gameArray[coin->posX+1][coin->posY]=!gameArray[coin->posX+1][coin->posY];
                    }

                    if(coin->posX-1>=0){
                        // 翻转左侧金币
                        btnCoin[coin->posX-1][coin->posY]->changeFlag();
                        gameArray[coin->posX-1][coin->posY]=!gameArray[coin->posX-1][coin->posY];
                    }

                    if(coin->posY+1<4){
                        // 翻转上侧金币
                        btnCoin[coin->posX][coin->posY+1]->changeFlag();
                        gameArray[coin->posX][coin->posY+1]=!gameArray[coin->posX][coin->posY+1];
                    }

                    if(coin->posY-1>=0){
                        // 翻转下侧金币
                        btnCoin[coin->posX][coin->posY-1]->changeFlag();
                        gameArray[coin->posX][coin->posY-1]=!gameArray[coin->posX][coin->posY-1];
                    }

                    // 金币翻完后，判断是否胜利
                    this->isWin=true;
                    for(int i = 0; i < 4; i++){
                        for(int j= 0; j < 4; j++){
                            if(btnCoin[i][j]->flag==false)
                            {
                                isWin=false;
                                break;
                            }
                        }
                    }
                    if(isWin){
                        for(int i = 0; i < 4; i++){
                            for(int j= 0; j < 4; j++){
                               btnCoin[i][j]->isWin=true;
                            }
                        }

                        levelWin->play();
                        // 胜利动画
                        QPropertyAnimation *animation1=new QPropertyAnimation(winLabel,"geometry");
                        animation1->setDuration(1000);
                        animation1->setStartValue(QRect(winLabel->x(),winLabel->y(),winLabel->width(),winLabel->height()));
                        animation1->setEndValue(QRect(winLabel->x(),winLabel->y()+124, winLabel->width(),winLabel->height()));
                        animation1->setEasingCurve(QEasingCurve::OutBounce);
                        animation1->start();
                    }
                });
            });
        }
    }
}

void PlayScene::paintEvent(QPaintEvent *){
    // 创建画家，指定绘图设备
    QPainter painter(this);
    // 创建QPixmap对象
    QPixmap pix;

    // 加载图片
    pix.load(":/res/PlayLevelSceneBg.png");
    // 绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    // 加载标题
    pix.load(":/res/Title.png");
    // 缩放图片
    //pix=pix.scaled(pix.width()*0.5, pix.height()*0.5);
    // 绘制标题
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
