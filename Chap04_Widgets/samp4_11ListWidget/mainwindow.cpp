#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tBtnListIni->setDefaultAction(ui->actListIni);
    ui->tBtnListAppend->setDefaultAction(ui->actListAppend);
    ui->tBtnListClear->setDefaultAction(ui->actListClear);
    ui->tBtnListDelete->setDefaultAction(ui->actListDelete);
    ui->tBtnListInsert->setDefaultAction(ui->actListInsert);

    ui->tBtnSelALL->setDefaultAction(ui->actSelAll);
    ui->tBtnSelInvs->setDefaultAction(ui->actSelInvs);
    ui->tBtnSelNone->setDefaultAction(ui->actSelNone);

    QMenu *menuSelection=new QMenu(this);
    menuSelection->addAction(ui->actSelAll);
    menuSelection->addAction(ui->actSelInvs);
    menuSelection->addAction(ui->actSelNone);
    ui->tBtnSelectItem->setPopupMode(QToolButton::MenuButtonPopup);
    ui->tBtnSelectItem->setMenu(menuSelection);

    QToolButton *aBtn=new QToolButton(this);
    aBtn->setPopupMode(QToolButton::InstantPopup);
    aBtn->setText("选择选项");
    aBtn->setIcon(QIcon(":/images/icons/424.bmp"));
    aBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    aBtn->setMenu(menuSelection);
    ui->toolBar->addWidget(aBtn);

    ui->toolBar->addSeparator();
    // 退出
    ui->toolBar->addAction(ui->actExit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化列表
void MainWindow::on_actListIni_triggered()
{
    ui->listWidget->clear();
    QListWidgetItem *aItem;
    QIcon aIcon;
    aIcon.addFile(":/images/icons/check2.ico");
    bool chk=ui->chkBoxListEditable->isChecked();
    for (int i = 0; i < 10; ++i) {
        aItem=new QListWidgetItem();
        aItem->setText("Item"+QString::number(i));
        aItem->setIcon(aIcon);
        aItem->setCheckState(Qt::Checked);

        if(chk){
            aItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
        else {
            aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        }
        ui->listWidget->addItem(aItem);
    }
}

// 清除
void MainWindow::on_actListClear_triggered()
{
    ui->listWidget->clear();
}

// 插入
void MainWindow::on_actListInsert_triggered()
{
    QIcon aIcon(":/images/icons/check2.ico");
    bool chk=ui->chkBoxListEditable->isChecked();
    QListWidgetItem *aItem=new QListWidgetItem();
    aItem->setText("Item Inserted");
    aItem->setIcon(aIcon);
    aItem->setCheckState(Qt::Checked);

    if(chk){
        aItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    else {
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    ui->listWidget->insertItem(ui->listWidget->currentRow(),aItem);
}


// 添加
void MainWindow::on_actListAppend_triggered()
{
    QIcon aIcon(":/images/icons/check2.ico");
    bool chk=ui->chkBoxListEditable->isChecked();
    QListWidgetItem *aItem=new QListWidgetItem();
    aItem->setText("Item added");
    aItem->setIcon(aIcon);
    aItem->setCheckState(Qt::Checked);

    if(chk){
        aItem->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    else {
        aItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
    }
    ui->listWidget->addItem(aItem);
}

// 删除
void MainWindow::on_actListDelete_triggered()
{
    int row=ui->listWidget->currentRow();
    delete ui->listWidget->takeItem(row);
}

// 全选
void MainWindow::on_actSelAll_triggered()
{
    int cnt=ui->listWidget->count();
    for (int i = 0; i < cnt; ++i) {
        ui->listWidget->item(i)->setCheckState(Qt::Checked);
    }
}

// 全空
void MainWindow::on_actSelNone_triggered()
{
    int cnt=ui->listWidget->count();
    for (int i = 0; i < cnt; ++i) {
        ui->listWidget->item(i)->setCheckState(Qt::Unchecked);
    }
}

// 反选
void MainWindow::on_actSelInvs_triggered()
{
    int cnt=ui->listWidget->count();
    for (int i = 0; i < cnt; ++i) {
        QListWidgetItem *aItem= ui->listWidget->item(i);
        aItem->setCheckState(aItem->checkState()==Qt::Checked?Qt::Unchecked:Qt::Checked);
    }
}


void MainWindow::on_actExit_triggered()
{
    this->close();
}


void MainWindow::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    QString str;
    if(current!=NULL){
        if(previous==NULL)
            str="当前项："+current->text();
        else {
            str="前一项："+previous->text()+";"+"当前项："+current->text();
        }

        ui->editCutItemText->setText(str);
    }

    ui->plainTextEdit->appendPlainText("currentItemChanged()信号被发射");
}


void MainWindow::on_chkBoxSorting_clicked(bool checked)
{
    ui->listWidget->setSortingEnabled(checked);
}

// 升序排序
void MainWindow::on_tBtnSortAsc_clicked()
{
    ui->listWidget->sortItems(Qt::AscendingOrder);
}


void MainWindow::on_tBtnSortDes_clicked()
{
    ui->listWidget->sortItems(Qt::DescendingOrder);
}


void MainWindow::on_tBtnClearText_clicked()
{
    ui->plainTextEdit->clear();
}


void MainWindow::on_tBtnAddLine_clicked()
{
    ui->plainTextEdit->appendPlainText("");
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ui->plainTextEdit->appendPlainText("itemDoubleClicked："+item->text());
}

// 右键菜单
void MainWindow::on_listWidget_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos);
    QMenu *menuList=new QMenu();
    menuList->addAction(ui->actListIni);
    menuList->addAction(ui->actListAppend);
    menuList->addAction(ui->actListClear);
    menuList->addAction(ui->actListDelete);
    menuList->addAction(ui->actListInsert);
    menuList->addSeparator();
    menuList->addAction(ui->actSelAll);
    menuList->addAction(ui->actSelNone);
    menuList->addAction(ui->actSelInvs);

    menuList->exec(QCursor::pos());
    delete menuList;
}

