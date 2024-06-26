﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "tdialogdata.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tableView);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::selectData(){
    qryModel=new QSqlQueryModel(this);
    selModel=new QItemSelectionModel(qryModel,this);
    ui->tableView->setModel(qryModel);
    ui->tableView->setSelectionModel(selModel);

    QString str="SELECT empNo, Name, Gender, Birthday, Province, Department, Salary FROM employee ORDER BY empNo";
    qryModel->setQuery(str);

    if(qryModel->lastError().isValid()){
        QMessageBox::information(this,"错误","数据库读取表格失败！\n"+qryModel->lastError().text());
        return;
    }

    // 处理表头
    QSqlRecord rec=qryModel->record();
    qryModel->setHeaderData(rec.indexOf("empNo"),Qt::Horizontal,"工号");
    qryModel->setHeaderData(rec.indexOf("Name"),Qt::Horizontal,"姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"),Qt::Horizontal,"性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"),Qt::Horizontal,"出生日期");
    qryModel->setHeaderData(rec.indexOf("Province"),Qt::Horizontal,"省份");
    qryModel->setHeaderData(rec.indexOf("Department"),Qt::Horizontal,"部门");
    qryModel->setHeaderData(rec.indexOf("Salary"),Qt::Horizontal,"工资");

    ui->actOpenDB->setEnabled(false);
    ui->actRecDelete->setEnabled(true);
    ui->actRecEdit->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actScan->setEnabled(true);

}

void MainWindow::updateRecord(int recNo)
{
    QSqlRecord curRec=qryModel->record(recNo);
    int empNo=curRec.value("EmpNo").toInt();

    QSqlQuery query;
    query.prepare("select * from Employee where EmpNo=:EmpNo");
    query.bindValue(":EmpNo",empNo);
    query.exec();
    query.first();
    if(!query.isValid())
        return;

    TDialogData *dataDialog=new TDialogData(this);
    dataDialog->setRecord(query.record());
    if(dataDialog->exec()==QDialog::Accepted){
        QSqlRecord recData=dataDialog->record();
        query.prepare("update employee set Name=:Name,Gender=:Gender, Birthday=:Birthday, Province=:Province, "
                      "Department=:Department, Salary=:Salary, Memo=:Memo, Photo=:Photo where EmpNo=:EmpNo");

        query.bindValue(":EmpNo",recData.value("EmpNo"));
        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Birthday",recData.value("Birthday"));
        query.bindValue(":Province",recData.value("Province"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
        query.bindValue(":Photo",recData.value("Photo"));

        if(!query.exec())
            QMessageBox::critical(this,"error","更新数据错误\n"+query.lastError().text());
        else
        {
            qryModel->setQuery(qryModel->query().executedQuery());
        }

    }
}

void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件",QCoreApplication::applicationDirPath(),"SQLite数据库(*.db3)");
    if(aFile.isEmpty())
        return;

    DB=QSqlDatabase::addDatabase("QSQLITE");
    DB.setDatabaseName(aFile);
    if(DB.open())
        selectData();
    else
        QMessageBox::critical(this,"失败","打开数据库失败");
}


void MainWindow::on_actRecInsert_triggered()
{
    QSqlQuery query;
    query.exec("select * from employee where EmpNo=-1");
    QSqlRecord curRec=query.record();   // 获得一条空数据
    curRec.setValue("EmpNo", 3000+qryModel->rowCount());

    TDialogData *dataDialog=new TDialogData(this);
    dataDialog->setRecord(curRec);
    if(dataDialog->exec()==QDialog::Accepted){
        QSqlRecord recData=dataDialog->record();
        query.prepare("insert into employee(EmpNo, Name,Gender, Birthday, Province, Department, Salary, Memo, Photo) "
                      "values(:EmpNo, :Name, :Gender,:Birthday,:Province,:Department, :Salary, :Memo,:Photo)");

        query.bindValue(":EmpNo",recData.value("EmpNo"));
        query.bindValue(":Name",recData.value("Name"));
        query.bindValue(":Gender",recData.value("Gender"));
        query.bindValue(":Birthday",recData.value("Birthday"));
        query.bindValue(":Province",recData.value("Province"));
        query.bindValue(":Department",recData.value("Department"));
        query.bindValue(":Salary",recData.value("Salary"));
        query.bindValue(":Memo",recData.value("Memo"));
        query.bindValue(":Photo",recData.value("Photo"));

        if(!query.exec())
            QMessageBox::critical(this,"error","插入数据错误\n"+query.lastError().text());
        else
            qryModel->setQuery(qryModel->query().executedQuery());
    }
    delete dataDialog;
}


void MainWindow::on_actRecEdit_triggered()
{
    int curRecNo=selModel->currentIndex().row();
    updateRecord(curRecNo);
}


void MainWindow::on_tableView_doubleClicked(const QModelIndex &index)
{
    updateRecord(index.row());
}


void MainWindow::on_actRecDelete_triggered()
{
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord curRec=qryModel->record(curRecNo);
    if(curRec.isEmpty())
        return;

    int empNo=curRec.value("EmpNo").toInt();
    QSqlQuery query;
    query.prepare("delete from employee where EmpNo=:ID");
    query.bindValue(":ID",empNo);
    if(!query.exec())
        QMessageBox::critical(this,"error","删除失败\n"+query.lastError().text());
    else
        qryModel->setQuery(qryModel->query().executedQuery());

}


void MainWindow::on_actScan_triggered()
{
    QSqlQuery qryUpdate;
    qryUpdate.exec("update employee set Salary=Salary+1000");
    qryModel->setQuery(qryModel->query().lastQuery());
}

