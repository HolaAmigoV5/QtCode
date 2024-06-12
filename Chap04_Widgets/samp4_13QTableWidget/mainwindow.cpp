#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDate>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    labCellIndex=new QLabel("当前单元坐标：", this);
    labCellIndex->setMinimumWidth(250);
    labCellType=new QLabel("当前单元格类型",this);
    labCellType->setMinimumWidth(200);
    labStudId=new QLabel("学生ID", this);
    labStudId->setMinimumWidth(200);

    ui->statusBar->addWidget(labCellIndex);
    ui->statusBar->addWidget(labCellType);
    ui->statusBar->addWidget(labStudId);

    on_btnSetHeader_clicked();
    on_btnSetRows_clicked();
    on_btnIniData_clicked();
    on_btnAutoHeight_clicked();
}

MainWindow::~MainWindow()
{
    delete ui;
}


// 设置表头
void MainWindow::on_btnSetHeader_clicked()
{
    ui->tableWidget->clear();
    QStringList headerText;
    headerText<<"姓名"<<"性别"<<"出生日期"<<"民族"<<"分数"<<"是否党员";
    ui->tableWidget->setColumnCount(headerText.size());
    //ui->tableWidget->setHorizontalHeaderLabels(headerText);

    for(int i=0; i<ui->tableWidget->columnCount(); i++){
        QTableWidgetItem *headerItem=new QTableWidgetItem(headerText.at(i));
        QFont font=headerItem->font();
        font.setBold(true);
        font.setPointSize(11);
        headerItem->setFont(font);
        headerItem->setForeground(QBrush(Qt::red));
        ui->tableWidget->setHorizontalHeaderItem(i,headerItem);
    }
}


// 设置行数
void MainWindow::on_btnSetRows_clicked()
{
    ui->tableWidget->setRowCount(ui->spinRowCount->value());
}

// 初始化表格数据
void MainWindow::on_btnIniData_clicked()
{
    ui->tableWidget->clearContents();
    QDate birth(2000,2,2);
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString strName=QString("学生%1").arg(i);
        QString strSex=i%2==0?"男生":"女生";
        bool isParty=(i%2)?false:true;
        int score=QRandomGenerator::global()->bounded(60,100);

        createItemARow(i, strName, strSex,birth,"汉族",score,isParty);
        birth=birth.addDays(QRandomGenerator::global()->bounded(1,365));
    }
}

void MainWindow::createItemARow(int rowNo, QString name, QString sex, QDate birth, QString nation, int score, bool isParty){
    uint studID=20221000;
    //姓名
    QTableWidgetItem *item=new QTableWidgetItem(name, ctName);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    studID+=rowNo;
    item->setData(Qt::UserRole,QVariant(studID));
    ui->tableWidget->setItem(rowNo,colName,item);

    //性别
    QIcon icon;
    icon.addFile(sex=="男生"?":/icons/boy.ico":":/icons/girl.ico");

    item=new QTableWidgetItem(sex, ctSex);
    item->setIcon(icon);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colSex,item);

    //出生日期
    item=new QTableWidgetItem(birth.toString("yyyy-MM-dd"), ctBirth);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colBirth,item);

    //民族
    item=new QTableWidgetItem(nation, ctName);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colNation,item);

    //分数
    item=new QTableWidgetItem(QString::number(score), ctScore);
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colScore,item);

    // 是否党员
    item=new QTableWidgetItem("党员",ctPartyM);
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled|Qt::ItemIsUserCheckable);

    item->setCheckState(isParty==true?Qt::Checked:Qt::Unchecked);
    item->setBackground(QBrush(Qt::yellow));
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNo,colIsParty,item);
}

// 插入行
void MainWindow::on_btnInsertRow_clicked()
{
    int curRow=ui->tableWidget->currentRow();
    ui->tableWidget->insertRow(curRow);
    createItemARow(curRow,"新学生","男生",QDate::fromString("2012-02-01","yyyy-MM-dd"),"回族",65,true);
}

// 添加行
void MainWindow::on_btnAddRow_clicked()
{
    int curRow=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(curRow);
    createItemARow(curRow,"新学生2","女生",QDate::fromString("2012-02-01","yyyy-MM-dd"),"藏族",85,true);
}

// 删除行
void MainWindow::on_btnRemoveRow_clicked()
{
    int curRow=ui->tableWidget->currentRow();
    ui->tableWidget->removeRow(curRow);
}

// 自动高度
void MainWindow::on_btnAutoHeight_clicked()
{
    ui->tableWidget->resizeRowsToContents();
}

// 自动宽度
void MainWindow::on_btnAutoWidth_clicked()
{
    ui->tableWidget->resizeColumnsToContents();
}



// 读取表中数据
void MainWindow::on_btnReadText_clicked()
{
    ui->plainTextEdit->clear();
    if(ui->tableWidget->rowCount()<1 || ui->tableWidget->columnCount()<1)
        return;
    qDebug()<<"行数："<<ui->tableWidget->rowCount()<<"列数："<<ui->tableWidget->columnCount();

    QTableWidgetItem *item;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString str=QString ::asprintf("第%d行 ",i+1);
        for (int j = 0; j < ui->tableWidget->columnCount()-1; ++j) {
            item=ui->tableWidget->item(i,j);
            str+=item->text()+" ";
        }

        item=ui->tableWidget->item(i,colIsParty);
        if(item->checkState()==Qt::Checked)
            str+="党员";
        else
            str+="群众";

         ui->plainTextEdit->appendPlainText(str);
    }
}


void MainWindow::on_chkEdiable_clicked(bool checked)
{
    if(checked)
        ui->tableWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::SelectedClicked);
    else
        ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void MainWindow::on_chkRowColor_clicked(bool checked)
{
    ui->tableWidget->setAlternatingRowColors(checked);
}

// 显示水平表头
void MainWindow::on_chkShowHHeader_clicked(bool checked)
{
    ui->tableWidget->horizontalHeader()->setVisible(checked);
}

// 显示垂直表头
void MainWindow::on_chkShowVHeader_clicked(bool checked)
{
    ui->tableWidget->verticalHeader()->setVisible(checked);
}

// 行选择
void MainWindow::on_rBtnRowSelect_clicked()
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

// 单元格选择
void MainWindow::on_rBtnCellSelect_clicked()
{
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
}


void MainWindow::on_tableWidget_currentCellChanged(int currentRow, int currentColumn, int previousRow, int previousColumn)
{
    Q_UNUSED(previousRow); Q_UNUSED(previousColumn);
    QTableWidgetItem *item=ui->tableWidget->item(currentRow,currentColumn);
    if(item==nullptr)
        return;

    labCellIndex->setText(QString::asprintf("当前单元格坐标：%d, %d", currentRow, currentColumn));
    labCellType->setText(QString::asprintf("当前单元格类型：%d", item->type()));
    item=ui->tableWidget->item(currentRow,colName);
    labStudId->setText(QString::asprintf("学生ID: %d",item->data(Qt::UserRole).toUInt()));
}

