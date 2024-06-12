#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>

void MainWindow::iniModelData(QStringList &aFileContent)
{
    int rowCnt=aFileContent.size();
    m_model->setRowCount(rowCnt-1);

    QString header=aFileContent.at(0);
    static QRegularExpression re("\\s+");
    QStringList headerList=header.split(re,Qt::SkipEmptyParts);
    m_model->setHorizontalHeaderLabels(headerList);

    QStandardItem *aItem;
    int j;

    for(int i=1;i<rowCnt; ++i){
        QString aLineText=aFileContent.at(i);
        QStringList tmpList=aLineText.split(re,Qt::SkipEmptyParts);

        for(j=0;j<fixedColumnCount-1; ++j){
            aItem=new QStandardItem(tmpList[j]);
            m_model->setItem(i-1,j,aItem);
        }

        aItem=new QStandardItem(headerList.at(j));
        aItem->setCheckable(true);
        aItem->setBackground(QBrush(Qt::yellow));
        if(tmpList[j]=="0")
            aItem->setCheckState(Qt::Unchecked);
        else
            aItem->setCheckState(Qt::Checked);

        m_model->setItem(i-1,j,aItem);
    }
}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(current.isValid()){
        labCellPos->setText(QString::asprintf("当前单元格：%d行, %d列", current.row(), current.column()));
        QStandardItem *aItem=m_model->itemFromIndex(current);
        labCellText->setText("单元内容："+aItem->text());

        ui->actFontBold->setChecked(aItem->font().bold());
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labCurFile=new QLabel("当前文件",this);
    labCurFile->setMinimumWidth(200);
    labCellPos=new QLabel("单元格位置：",this);
    labCellPos->setMinimumWidth(200);
    labCellText=new QLabel("单元格内容：",this);
    labCellText->setMinimumWidth(200);

    ui->statusbar->addWidget(labCurFile);
    ui->statusbar->addWidget(labCellPos);
    ui->statusbar->addWidget(labCellText);

    m_model=new QStandardItemModel(2,fixedColumnCount,this);
    m_selectionModel=new QItemSelectionModel(m_model,this);
    ui->tableView->setModel(m_model);
    ui->tableView->setSelectionModel(m_selectionModel);
    ui->tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectItems);

    connect(m_selectionModel,&QItemSelectionModel::currentChanged,this,&MainWindow::do_currentChanged);


    m_SpinDelegate=new TSpinBoxDelegate(this);
    ui->tableView->setItemDelegateForColumn(0,m_SpinDelegate);

    m_FloatSpinDelegate=new TFloatSpindelegate(this);
    ui->tableView->setItemDelegateForColumn(1,m_FloatSpinDelegate);
    ui->tableView->setItemDelegateForColumn(2,m_FloatSpinDelegate);
    ui->tableView->setItemDelegateForColumn(3,m_FloatSpinDelegate);

    m_comboBoxDelegate=new TComboBoxDelegate(this);
    QStringList strList;
    strList<<"优"<<"良"<<"中"<<"差";
    m_comboBoxDelegate->setItems(strList,false);
    ui->tableView->setItemDelegateForColumn(4,m_comboBoxDelegate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 数据预览
void MainWindow::on_actDataPreView_triggered()
{
    ui->plainTextEdit->clear();
    QStandardItem *aItem;
    QString str;

    // 处理表头
    for (int i = 0; i < m_model->columnCount(); ++i) {
        aItem=m_model->horizontalHeaderItem(i);
        str+=aItem->text()+"\t";
    }
    ui->plainTextEdit->appendPlainText(str);

    // 处理数据
    for(int i=0; i<m_model->rowCount();++i){
        str="";
        int j;
        for(j=0; j<m_model->columnCount()-1; ++j){
            aItem=m_model->item(i,j);
            str+=aItem->text()+"\t";
        }

         aItem=m_model->item(i,j);
        if(aItem->checkState()==Qt::Checked)
             str+="是";
        else
            str+="否";
        ui->plainTextEdit->appendPlainText(str);
    }
}

// 打开文件
void MainWindow::on_actOpenFile_triggered()
{
    QString curPath=QCoreApplication::applicationDirPath();
    QString aFileName=QFileDialog::getOpenFileName(this, "打开文件",curPath,"数据文件(*.txt);;所有文件(*.*)");

    if(aFileName.isEmpty())
        return;

    QFile aFile(aFileName);
    if(!aFile.open(QIODevice::ReadOnly|QIODevice::Text))
        return;

    QStringList aFileContent;
    ui->plainTextEdit->clear();
    QTextStream aStream(&aFile);
    while (!aStream.atEnd()) {
        QString str=aStream.readLine();
        ui->plainTextEdit->appendPlainText(str);
        aFileContent.append(str);
    }
    aFile.close();

    labCurFile->setText("当前文件："+curPath);
    ui->actAppend->setEnabled(true);
    ui->actInsert->setEnabled(true);
    ui->actDelete->setEnabled(true);

    iniModelData(aFileContent);
}

// 添加行
void MainWindow::on_actAppend_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for (int i = 0; i < m_model->columnCount()-1; ++i) {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    QString str=m_model->headerData(m_model->columnCount()-1, Qt::Horizontal).toString();
    aItem=new QStandardItem(str);
    aItem->setCheckable(true);
    aItem->setBackground(QBrush(Qt::yellow));
    aItemList<<aItem;
    m_model->insertRow(m_model->rowCount(),aItemList);

    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(m_model->index(m_model->rowCount()-1,0),QItemSelectionModel::Select);
}

// 插入行
void MainWindow::on_actInsert_triggered()
{
    QList<QStandardItem*> aItemList;
    QStandardItem *aItem;
    for (int i = 0; i < m_model->columnCount()-1; ++i) {
        aItem=new QStandardItem("0");
        aItemList<<aItem;
    }
    QString str=m_model->headerData(m_model->columnCount()-1, Qt::Horizontal).toString();
    aItem=new QStandardItem(str);
    aItem->setCheckable(true);
    aItem->setBackground(QBrush(Qt::yellow));
    aItemList<<aItem;

    QModelIndex curIndex=m_selectionModel->currentIndex();
    m_model->insertRow(curIndex.row(),aItemList);

    m_selectionModel->clearSelection();
    m_selectionModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}

// 删除行
void MainWindow::on_actDelete_triggered()
{
    QModelIndex curIndex=m_selectionModel->currentIndex();

    if(curIndex.row()!=m_model->rowCount()-1)
        m_selectionModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
    m_model->removeRow(curIndex.row());
}


void MainWindow::on_actAlignLeft_triggered()
{
    if(!m_selectionModel->hasSelection())
        return;

    QModelIndexList indexList=m_selectionModel->selectedIndexes();
    for(auto aIndex:indexList){
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    }
}


void MainWindow::on_actAlignCenter_triggered()
{
    if(!m_selectionModel->hasSelection())
        return;

    QModelIndexList indexList=m_selectionModel->selectedIndexes();
    for(auto aIndex:indexList){
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignCenter);
    }
}


void MainWindow::on_actAlignRight_triggered()
{
    if(!m_selectionModel->hasSelection())
        return;

    QModelIndexList indexList=m_selectionModel->selectedIndexes();
    for(auto aIndex:indexList){
        m_model->itemFromIndex(aIndex)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
    }
}




void MainWindow::on_actFontBold_triggered(bool checked)
{
    if(!m_selectionModel->hasSelection())
        return;

    QModelIndexList indexList=m_selectionModel->selectedIndexes();
    for(auto aIndex:indexList){
        QStandardItem *item=m_model->itemFromIndex(aIndex);
        QFont font= item->font();
        font.setBold(checked);
        item->setFont(font);
    }
}

