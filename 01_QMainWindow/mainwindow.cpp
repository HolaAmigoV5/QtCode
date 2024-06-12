#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>
#include <QDockWidget>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800,600);

    // 菜单栏创建，只能有一个
    QMenuBar *bar= menuBar();
    // 将菜单栏放入到窗口中
    setMenuBar(bar);

    // 创建菜单
    QMenu *fileMenu= bar->addMenu("文件");
    QMenu *editMenu= bar->addMenu("编辑");

    // 创建菜单项
    QAction *newAction= fileMenu->addAction("新建");
    // 添加分隔线
    fileMenu->addSeparator();
    QAction *openAction= fileMenu->addAction("打开");

    editMenu->addAction("新编辑");

    // 工具栏，可以有多个
    QToolBar *toolBar=new QToolBar(this);
    addToolBar(Qt::LeftToolBarArea,toolBar);

    // 只允许左右停靠
    toolBar->setAllowedAreas(Qt::LeftToolBarArea | Qt::RightToolBarArea);

    // 设置浮动
    toolBar->setFloatable(false);

    // 设置移动
    toolBar->setMovable(false);

    // 工具栏中可以设置内容
    toolBar->addAction(newAction);
    toolBar->addSeparator();
    toolBar->addAction(openAction);

    // 工具栏中添加控件
    QPushButton *btn=new QPushButton("复制", this);
    toolBar->addWidget(btn);

    // 状态栏
    QStatusBar *stBar = statusBar();
    // 设置到窗口
    setStatusBar(stBar);
    // 放置标签控件
    QLabel *label=new QLabel("提示信息：", this);
    stBar->addWidget(label);
    QLabel *rLabel=new QLabel("右侧提示信息：", this);
    stBar->addPermanentWidget(rLabel);


    // 铆接部件
    QDockWidget *dockWidget=new QDockWidget("浮动",this);
    addDockWidget(Qt::BottomDockWidgetArea, dockWidget);
    dockWidget->setAllowedAreas(Qt::BottomDockWidgetArea|Qt::TopDockWidgetArea);

    // 设置中心部件
    QTextEdit *edit=new QTextEdit(this);
    setCentralWidget(edit);
}

MainWindow::~MainWindow()
{
}

