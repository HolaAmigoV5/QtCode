#include "chooselevelscene.h"
#include <QMenuBar>
#include <QPainter>
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include <QSound>
#include "mypushbutton.h"

ChooseLevelScene::ChooseLevelScene(QWidget *parent) : QMainWindow(parent)
{
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

    // 音效
    QSound *chooseSound=new QSound(":res/TapButtonSound.wav",this);
    QSound *backSound=new QSound(":res/BackButtonSound.wav",this);

    // 返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width()*1.2,this->height()-backBtn->height()*1.2);
    connect(backBtn, &QPushButton::clicked,this,[=](){
        backSound->play();
        // 延时发送信号
        QTimer::singleShot(500, this, [=](){
            // 发送返回信号
            emit this-> chooseSceneBack();
        });
    });


    // 关卡按钮
    for(int i=0; i<20; i++){
        MyPushButton *levelBtn=new MyPushButton(":/res/LevelIcon.png");
        levelBtn->setParent(this);
        levelBtn->move(50+(i%4)*120, 130+(i/4)*120);
        connect(levelBtn, &MyPushButton::clicked, this, [=](){
            //qDebug()<<QString("选中了第 %1 关。").arg(i+1);

            chooseSound->play();
            if(pScene==NULL){
                this->hide();
                pScene=new PlayScene(i+1);

                pScene->setGeometry(this->geometry());
                pScene->show();     // 显示游戏场景

                // 监听返回信号
                connect(pScene, &PlayScene::playSceneBack, this, [=](){
                    this->setGeometry(pScene->geometry());
                    this->show();
                    delete pScene;
                    pScene=NULL;
                });
            }
        });

        // 按钮上显示文字
        QLabel *label=new QLabel;
        label->setParent(this);
        label->setFixedSize(levelBtn->width(),levelBtn->height());
        label->setText(QString::number(i+1));
        label->setAlignment(Qt::AlignCenter);
        label->move(50+(i%4)*120, 130+(i/4)*120);
        label->setAttribute(Qt::WA_TransparentForMouseEvents, true); // 鼠标事件穿透
    }
}

void ChooseLevelScene::paintEvent(QPaintEvent *){
    // 创建画家，指定绘图设备
    QPainter painter(this);
    // 创建QPixmap对象
    QPixmap pix;

    // 加载图片
    pix.load(":/res/OtherSceneBg.png");
    // 绘制背景图
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    // 加载标题
    pix.load(":/res/Title.png");
    // 缩放图片
    //pix=pix.scaled(pix.width()*0.5, pix.height()*0.5);
    // 绘制标题
    painter.drawPixmap(10,30,pix.width(),pix.height(),pix);
}
