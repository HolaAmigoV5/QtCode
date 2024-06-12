#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

void MainWindow::changeItemCaption(QTreeWidgetItem *item)
{
    QString str=">>"+ item->text(colItem);
    item->setText(colItem,str);
    if(item->childCount()>0){
        for(int i=0; i<item->childCount(); i++){
            changeItemCaption(item->child(i));
        }
    }
}

void MainWindow::displayImage(QTreeWidgetItem *item)
{
    QString filename=item->data(colItem,Qt::UserRole).toString();
    labFileName->setText(filename);
    labNodeText->setText(item->text(colItem));

    m_pixmap.load(filename);
    ui->actZoomFitH->triggered();

    ui->actZoomFitH->setEnabled(true);
    ui->actZoomFitW->setEnabled(true);
    ui->actZoomIn->setEnabled(true);
    ui->actZoomOut->setEnabled(true);
    ui->actZoomRealSize->setEnabled(true);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->treeWidget->clear();


    // 创建头
    QTreeWidgetItem *header=new QTreeWidgetItem();
    header->setText(colItem,"目录和文件");
    header->setText(colItemType,"节点类型");
    header->setText(colDate,"最后修改日期");

    header->setTextAlignment(colItem,Qt::AlignCenter);
    header->setTextAlignment(colItemType,Qt::AlignCenter);
    header->setTextAlignment(colDate,Qt::AlignCenter);

    ui->treeWidget->setHeaderItem(header);

    // 添加一个根节点
    QIcon icon(":/images/icons/15.ico");
    QTreeWidgetItem *topItem=new QTreeWidgetItem(itTopItem);
    topItem->setIcon(colItem,icon);
    topItem->setText(colItem,"图片");
    topItem->setText(colItemType,"Top Item");
    topItem->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
    topItem->setCheckState(colItem,Qt::Checked);
    ui->treeWidget->addTopLevelItem(topItem);


    // status bar
    labNodeText=new QLabel("节点标题",this);
    labNodeText->setMinimumWidth(200);
    ui->statusbar->addWidget(labNodeText);

    spinRatio=new QSpinBox(this);
    spinRatio->setMinimumWidth(60);
    spinRatio->setRange(0,2000);
    spinRatio->setValue(100);
    spinRatio->setSuffix(" %");
    spinRatio->setReadOnly(true);
    spinRatio->setButtonSymbols(QAbstractSpinBox::NoButtons);
    ui->statusbar->addPermanentWidget(spinRatio);

    labFileName=new QLabel("文件名",this);
    ui->statusbar->addPermanentWidget(labFileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 添加目录
void MainWindow::on_actAddFolder_triggered()
{
    QString dir=QFileDialog::getExistingDirectory();
    if(dir.isEmpty())
        return;

    QTreeWidgetItem *parItem=ui->treeWidget->currentItem();
    if(parItem==nullptr)
        return;

    if(parItem->type()!=itImageItem){
        int cnt=dir.length();
        int i=dir.lastIndexOf("/");
        QString nodeName=dir.right(cnt-i-1);

        QTreeWidgetItem *item=new QTreeWidgetItem(itGroupItem);
        QIcon icon(":/images/icons/open3.bmp");
        item->setIcon(colItem,icon);
        item->setText(colItem,nodeName);
        item->setText(colItemType,"Group Item");
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
        item->setCheckState(colItem,Qt::Checked);
        item->setData(colItem,Qt::UserRole,QVariant(dir));
        parItem->addChild(item);
    }
    parItem->setExpanded(true);
}

// 添加文件
void MainWindow::on_actAddFiles_triggered()
{
    QStringList files=QFileDialog::getOpenFileNames(this, "选择文件","", "Images(*.jpg)");
    if(files.isEmpty())
        return;

    QTreeWidgetItem *parItem, *item;
    item=ui->treeWidget->currentItem();
    if(item==nullptr)
        item=ui->treeWidget->topLevelItem(0);

    if(item->type()==itImageItem)
        parItem=item->parent();
    else
        parItem=item;

    for (int i = 0; i < files.size(); ++i) {
        QString fileName=files.at(i);
        QIcon icon(":/images/icons/31.ico");
        QFileInfo fileInfo(fileName);
        QString NodeText=fileInfo.fileName();
        QDateTime dateTime=fileInfo.lastModified();

        QTreeWidgetItem *child=new QTreeWidgetItem(itImageItem);
        child->setIcon(colItem,icon);
        child->setText(colItem,NodeText);
        child->setText(colItemType, "Image Item");
        child->setText(colDate, dateTime.toString("yyyy-MM-dd"));
        child->setFlags(Qt::ItemIsSelectable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled|Qt::ItemIsAutoTristate);
        child->setCheckState(colItem,Qt::Checked);
        child->setData(colItem,Qt::UserRole, QVariant(fileName));
        parItem->addChild(child);
    }
    parItem->setExpanded(true);
}

// 删除
void MainWindow::on_actDeleteItem_triggered()
{
    QTreeWidgetItem *item=ui->treeWidget->currentItem();
    if(item==nullptr)
        return;

    QTreeWidgetItem *parItem=item->parent();
    parItem->removeChild(item);
}


// 选择项发生改变时
void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    if(current==nullptr|| current==previous)
        return;

    switch (current->type()) {
    case itTopItem:
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(false);
        break;
    case itGroupItem:
        ui->actAddFolder->setEnabled(true);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        break;
    case itImageItem:
        ui->actAddFolder->setEnabled(false);
        ui->actAddFiles->setEnabled(true);
        ui->actDeleteItem->setEnabled(true);
        // 显示图片
        displayImage(current);
        break;
    }
}


// 遍历
void MainWindow::on_actScanItems_triggered()
{
    for(int i=0; i<ui->treeWidget->topLevelItemCount();++i){
        changeItemCaption(ui->treeWidget->topLevelItem(i));
    }
}

// 按照合适的宽度缩放
void MainWindow::on_actZoomFitW_triggered()
{
    int w=ui->scrollArea->width()-30;
    int realw=m_pixmap.width();
    m_radio=float(w)/realw;
    spinRatio->setValue(m_radio*100);


    // 开始缩放
    QPixmap pix=m_pixmap.scaledToWidth(w-30);
    ui->labPic->setPixmap(pix);
}

// 按照合适的高度缩放
void MainWindow::on_actZoomFitH_triggered()
{
    int h=ui->scrollArea->height();
    int realh=m_pixmap.height();
    m_radio=float(h)/realh;
    spinRatio->setValue(m_radio*100);


    // 开始缩放
    QPixmap pix=m_pixmap.scaledToHeight(h-30);
    ui->labPic->setPixmap(pix);
}


// 原始大小
void MainWindow::on_actZoomRealSize_triggered()
{
    ui->labPic->setPixmap(m_pixmap);
    m_radio=1;
    spinRatio->setValue(100);

}

// 放大
void MainWindow::on_actZoomIn_triggered()
{
    m_radio*=1.2;
    int w=m_radio*m_pixmap.width();
    int h=m_radio*m_pixmap.height();

    ui->labPic->setPixmap( m_pixmap.scaled(w,h));

    spinRatio->setValue(100*m_radio);
}

// 缩小
void MainWindow::on_actZoomOut_triggered()
{
    m_radio*=0.8;
    int w=m_radio*m_pixmap.width();
    int h=m_radio*m_pixmap.height();

    ui->labPic->setPixmap( m_pixmap.scaled(w,h));
    spinRatio->setValue(100*m_radio);
}


void MainWindow::on_actDockVisible_triggered(bool checked)
{
    ui->dockWidget->setVisible(checked);
}


void MainWindow::on_actDockFloat_triggered(bool checked)
{
    ui->dockWidget->setFloating(checked);
}


void MainWindow::on_dockWidget_visibilityChanged(bool visible)
{
    ui->actDockVisible->setChecked(visible);
}


void MainWindow::on_dockWidget_topLevelChanged(bool topLevel)
{
    ui->actDockFloat->setChecked(topLevel);
}

