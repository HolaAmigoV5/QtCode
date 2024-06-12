#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTreeWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->rBtnMan->setChecked(true);

    // 选中女后，打印信息
    connect(ui->rBtnWoman, &QRadioButton::clicked,[=](){
        qDebug()<<"选中女的了";

        QMessageBox::information(this,"提示", "当前选择了女的");
    });

    // 利用ListWidget写诗
//    QListWidgetItem *item=new QListWidgetItem("锄禾日当午,");
//    ui->listWidget->addItem(item);
//    item->setTextAlignment(Qt::AlignHCenter);
//    ui->listWidget->addItem("汗滴禾下土。");


    QStringList list;
    list<<"锄禾日当午，"<<"汗滴禾下土。"<<"谁知盘中餐，"<<"粒粒皆辛苦。";
    ui->listWidget->addItems(list);


    // 树控件的使用
    // 设置头部标题
    ui->treeWidget->setHeaderLabels(QStringList()<<"英雄"<<"英雄介绍");

    // 加载顶层节点
    QTreeWidgetItem *liItem=new QTreeWidgetItem(QStringList()<<"力量");
    QTreeWidgetItem *mjItem=new QTreeWidgetItem(QStringList()<<"敏捷");
    QTreeWidgetItem *zlItem=new QTreeWidgetItem(QStringList()<<"智力");
    ui->treeWidget->addTopLevelItem(liItem);
    ui->treeWidget->addTopLevelItem(mjItem);
    ui->treeWidget->addTopLevelItem(zlItem);


    // 追加子节点
    QTreeWidgetItem *h1=new QTreeWidgetItem(QStringList()<<"刚背猪"<<"前排坦克，能在洗手伤害时造成可观的范围输出");
    liItem->addChild(h1);
    QTreeWidgetItem *h2=new QTreeWidgetItem(QStringList()<<"船长"<<"前排坦克，能在洗手伤害时造成可观的范围输出");
    liItem->addChild(h2);
    QTreeWidgetItem *h3=new QTreeWidgetItem(QStringList()<<"巫医"<<"前排坦克，能在洗手伤害时造成可观的范围输出");
    zlItem->addChild(h3);
}

MainWindow::~MainWindow()
{
    delete ui;
}

