#include "mainscene.h"
#include "ui_mainscene.h"
#include <QMenu>
#include <QPixmap>
#include <QPainter>
#include <QTimer>
#include <QSound>
#include "mypushbutton.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);

    // 配置主场景
    this->setFixedSize(520,788);    // 设置固定大小
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));
    this->setWindowTitle("伍哥赏金币");

    // 退出按钮功能实现
    connect(ui->actionquit,&QAction::triggered,this,[=](){
        this->close();
    });

    // 准备音效
    QSound *startSound = new QSound(":/res/TapButtonSound.wav",this);
    startSound->play();

    // 开始按钮
    MyPushButton *startBtn=new MyPushButton(QString(":/res/MenuSceneStartButton.png"));
    startBtn->setParent(this);
    startBtn->move((this->width()-startBtn->width())/2, this->height()*0.8);

    chooseScene=new ChooseLevelScene;

    // 监听信号
    connect(chooseScene,&ChooseLevelScene::chooseSceneBack, this, [=](){
        this->setGeometry(chooseScene->geometry());
        this->show();
        chooseScene->hide();
    });

    // 点击后弹跳特效，同时延时0.5秒后，切换页面
    connect(startBtn, &QPushButton::clicked,this, [=](){
        startSound->play();
        startBtn->zoom1();
        startBtn->zoom2();

        QTimer::singleShot(500, this, [=](){
            // 设置场景位置
            chooseScene->setGeometry(this->geometry());
            this->hide();
            chooseScene->show();
        });
    });
}

MainScene::~MainScene()
{
    delete ui;
}

void MainScene::paintEvent(QPaintEvent *){
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

