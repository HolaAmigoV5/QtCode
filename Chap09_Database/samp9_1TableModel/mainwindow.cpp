#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->splitter);

    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setAlternatingRowColors(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::UpdateStatusbar(){
     ui->statusbar->showMessage(QString("记录条数为：%1").arg(tableModel->rowCount()));
}

void MainWindow::RefreshTableView()
{
    int index=dataMapper->currentIndex();
    QModelIndex curIndex=qryModel->index(index,1);
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);
}

void MainWindow::on_actOpenDB_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择文件",QCoreApplication::applicationDirPath(),"SQLite数据库(*.db3)");
    if(aFile.isEmpty())
        return;


    DB=QSqlDatabase::addDatabase("QSQLITE");    // 添加驱动
    DB.setDatabaseName(aFile);
    if(!DB.open()){
        QMessageBox::warning(this,"错误","数据库打开失败!");
    }
    else{
        //openTable();    // 打开数据库表格
        selectData();
    }

}

void MainWindow::do_currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(current); Q_UNUSED(previous);
    ui->actSubmit->setEnabled(tableModel->isDirty());
    ui->actRevert->setEnabled(tableModel->isDirty());
}

void MainWindow::do_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(!current.isValid()){
        ui->dbLabPhoto->clear();
        return;
    }

    ui->actRecDelete->setEnabled(true);
    ui->actPhoto->setEnabled(true);
    ui->actPhotoClear->setEnabled(true);

    // 映射到widget
    dataMapper->setCurrentIndex(current.row());

    // 照片处理
    QSqlRecord curRec=tableModel->record(current.row());
    if(curRec.isNull("Photo"))
        ui->dbLabPhoto->clear();
    else{
        QPixmap pic;
        pic.loadFromData(curRec.value("Photo").toByteArray());
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->size().width()));
    }

}

void MainWindow::on_currentRowChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    if(!current.isValid()){
        ui->dbLabPhoto->clear();
        return;
    }

    ui->actRecDelete->setEnabled(true);
    ui->actPhoto->setEnabled(true);
    ui->actPhotoClear->setEnabled(true);

    // 映射到widget
    dataMapper->setCurrentModelIndex(current);
    bool first=(current.row()==0);
    bool last=(current.row()==qryModel->rowCount()-1);
    ui->actRecFirst->setEnabled(!first);
    ui->actRecPrevious->setEnabled(!first);
    ui->actRecLast->setEnabled(!last);
    ui->actRecNext->setEnabled(!last);

   // 获取photo和memo
    int curRecNo=selModel->currentIndex().row();
    QSqlRecord curRec=qryModel->record(curRecNo);
    int empNo=curRec.value("EmpNo").toInt();

    // 查询数据
    QSqlQuery query;
    query.prepare("select Memo, Photo from employee where EmpNo= :ID");
    query.bindValue(":ID",empNo);
    query.exec();
    query.first();

    QVariant va=query.value("Photo");
    if(!va.isValid()){
        ui->dbLabPhoto->clear();
    }
    else{
        QPixmap pic;
        pic.loadFromData(va.toByteArray());
        ui->dbLabPhoto->setPixmap(pic.scaledToWidth(ui->dbLabPhoto->width()));
    }

    QVariant va2=query.value("Memo");
    ui->dbEditMemo->setPlainText(va2.toString());
}

void MainWindow::openTable()
{
    // 创建模型，打开数据库表格
    tableModel=new QSqlTableModel(this,DB);
    tableModel->setTable("employee");
    tableModel->setEditStrategy(QSqlTableModel::OnManualSubmit);    // 手动提交
    tableModel->setSort(tableModel->fieldIndex("empNo"),Qt::AscendingOrder);

    if(!tableModel->select())
        QMessageBox::critical(this,"错误","数据库表格打开失败："+tableModel->lastError().text());

    UpdateStatusbar();
    //ui->statusbar->showMessage(QString("记录条数为：%1").arg(tableModel->rowCount()));

    // 设置标题
    tableModel->setHeaderData(tableModel->fieldIndex("empNo"),Qt::Horizontal, "工号");
    tableModel->setHeaderData(tableModel->fieldIndex("Name"),Qt::Horizontal, "姓名");
    tableModel->setHeaderData(tableModel->fieldIndex("Gender"),Qt::Horizontal, "性别");
    tableModel->setHeaderData(tableModel->fieldIndex("Birthday"),Qt::Horizontal, "出生日期");
    tableModel->setHeaderData(tableModel->fieldIndex("Province"),Qt::Horizontal, "省份");
    tableModel->setHeaderData(tableModel->fieldIndex("Department"),Qt::Horizontal, "部门");
    tableModel->setHeaderData(tableModel->fieldIndex("Salary"),Qt::Horizontal, "工资");

    //tableModel->setHeaderData(tableModel->fieldIndex("Memo"),Qt::Horizontal, "备注");
    //tableModel->setHeaderData(tableModel->fieldIndex("Photo"),Qt::Horizontal, "照片");


    // model/view
    selModel=new QItemSelectionModel(tableModel,this);


    ui->tableView->setModel(tableModel);
    ui->tableView->setSelectionModel(selModel);

    ui->tableView->setColumnHidden(tableModel->fieldIndex("Memo"),true);
    ui->tableView->setColumnHidden(tableModel->fieldIndex("Photo"),true);

    // 代理
    QStringList strList;
    strList<<"男"<<"女";
    delegateSex.SetItems(strList,false);
    ui->tableView->setItemDelegateForColumn(tableModel->fieldIndex("Gender"), &delegateSex);

    strList.clear();
    strList<<"技术部"<<"销售部"<<"市场部"<<"行政部";
    delegateDepart.SetItems(strList,false);
    ui->tableView->setItemDelegateForColumn(tableModel->fieldIndex("Department"), &delegateDepart);

    // 字段与widget映射
    dataMapper=new QDataWidgetMapper(this);
    dataMapper->setModel(tableModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataMapper->addMapping(ui->dbSpinEmpNo,tableModel->fieldIndex("empNo"));
    dataMapper->addMapping(ui->dbEditName,tableModel->fieldIndex("Name"));
    dataMapper->addMapping(ui->dbComboSex,tableModel->fieldIndex("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,tableModel->fieldIndex("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,tableModel->fieldIndex("Province"));
    dataMapper->addMapping(ui->dbComboDep,tableModel->fieldIndex("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,tableModel->fieldIndex("Salary"));
    dataMapper->addMapping(ui->dbEditMemo,tableModel->fieldIndex("Memo"));
    dataMapper->toFirst();

    // action状态
    ui->actOpenDB->setEnabled(false);
    ui->actRecAppend->setEnabled(true);
    ui->actRecInsert->setEnabled(true);
    ui->actRecDelete->setEnabled(true);
    ui->actScan->setEnabled(true);

    ui->groupBoxSort->setEnabled(true);
    ui->groupBoxFilter->setEnabled(true);

    // 获取字段名，更新ComboBox
    QSqlRecord emptyRec= tableModel->record();
    for(int i=0; i<emptyRec.count(); ++i)
        ui->comboFields->addItem(emptyRec.fieldName(i));

    connect(selModel, &QItemSelectionModel::currentChanged,this,&MainWindow::do_currentChanged);
    connect(selModel, &QItemSelectionModel::currentRowChanged, this, &MainWindow::do_currentRowChanged);
}

void MainWindow::selectData()
{
    QString str="SELECT empNo, Name, Gender, Birthday, Province, Department, Salary FROM employee ORDER BY empNo";
    qryModel=new QSqlQueryModel(this);
    qryModel->setQuery(str);
    if(qryModel->lastError().isValid()){
        QMessageBox::critical(this,"错误","数据库查询错误\n"+qryModel->lastError().text());
        return;
    }

    ui->statusbar->showMessage(QString("记录条数为：%1").arg(qryModel->rowCount()));
    ui->tableView->setModel(qryModel);
    selModel=new QItemSelectionModel(qryModel, this);
    ui->tableView->setSelectionModel(selModel);

    // 处理表头
    QSqlRecord rec=qryModel->record();
    qryModel->setHeaderData(rec.indexOf("empNo"),Qt::Horizontal,"工号");
    qryModel->setHeaderData(rec.indexOf("Name"),Qt::Horizontal,"姓名");
    qryModel->setHeaderData(rec.indexOf("Gender"),Qt::Horizontal,"性别");
    qryModel->setHeaderData(rec.indexOf("Birthday"),Qt::Horizontal,"出生日期");
    qryModel->setHeaderData(rec.indexOf("Province"),Qt::Horizontal,"省份");
    qryModel->setHeaderData(rec.indexOf("Department"),Qt::Horizontal,"部门");
    qryModel->setHeaderData(rec.indexOf("Salary"),Qt::Horizontal,"工资");

    // 字段与widget映射
    dataMapper=new QDataWidgetMapper(this);
    dataMapper->setModel(qryModel);
    dataMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);

    dataMapper->addMapping(ui->dbSpinEmpNo,rec.indexOf("empNo"));
    dataMapper->addMapping(ui->dbEditName,rec.indexOf("Name"));
    dataMapper->addMapping(ui->dbComboSex,rec.indexOf("Gender"));
    dataMapper->addMapping(ui->dbEditBirth,rec.indexOf("Birthday"));
    dataMapper->addMapping(ui->dbComboProvince,rec.indexOf("Province"));
    dataMapper->addMapping(ui->dbComboDep,rec.indexOf("Department"));
    dataMapper->addMapping(ui->dbSpinSalary,rec.indexOf("Salary"));


    dataMapper->toFirst();

    ui->actOpenDB->setEnabled(false);

    connect(selModel, &QItemSelectionModel::currentRowChanged,this,&MainWindow::on_currentRowChanged);
}


void MainWindow::on_actRecAppend_triggered()
{
    QSqlRecord rec=tableModel->record();
    rec.setValue(tableModel->fieldIndex("Gender"),"男");
    tableModel->insertRecord(tableModel->rowCount(),rec);
    selModel->clearSelection();
    selModel->setCurrentIndex(tableModel->index(tableModel->rowCount()-1,1),QItemSelectionModel::Select);

    UpdateStatusbar();
    //ui->statusbar->showMessage(QString("记录条数为：%1").arg(tableModel->rowCount()));
}


void MainWindow::on_actRecInsert_triggered()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    QSqlRecord rec=tableModel->record();
    rec.setValue(tableModel->fieldIndex("Gender"),"男");
    tableModel->insertRecord(curIndex.row(),rec);
    selModel->clearSelection();
    selModel->setCurrentIndex(curIndex,QItemSelectionModel::Select);

    UpdateStatusbar();
    //ui->statusbar->showMessage(QString("记录条数为：%1").arg(tableModel->rowCount()));
}


void MainWindow::on_actRecDelete_triggered()
{
    QModelIndex curIndex=ui->tableView->currentIndex();
    tableModel->removeRow(curIndex.row());
    UpdateStatusbar();
    //ui->statusbar->showMessage(QString("记录条数为：%1").arg(tableModel->rowCount()));
}


void MainWindow::on_actPhoto_triggered()
{
    QString aFile=QFileDialog::getOpenFileName(this,"选择一个图片","","JPG照片(*.jpg);;PNG照片(*.png)");
    if(aFile.isEmpty())
        return;

    QFile file(aFile);
    if(file.open(QIODevice::ReadOnly)){
        QByteArray data=file.readAll();
        file.close();

        QSqlRecord curRec=tableModel->record(selModel->currentIndex().row());
        curRec.setValue("Photo",data);
        tableModel->setRecord(selModel->currentIndex().row(),curRec);

        QPixmap pix;
        pix.load(aFile);
        ui->dbLabPhoto->setPixmap(pix.scaledToWidth(ui->dbLabPhoto->width()));
    }
}


void MainWindow::on_actPhotoClear_triggered()
{
    QSqlRecord curRec=tableModel->record(selModel->currentIndex().row());
    curRec.setNull("Photo");
    tableModel->setRecord(selModel->currentIndex().row(),curRec);
    ui->dbLabPhoto->clear();
}


void MainWindow::on_actScan_triggered()
{
    if(tableModel->rowCount()==0)
        return;

    for(int i=0; i<tableModel->rowCount(); ++i){
        QSqlRecord aRec=tableModel->record(i);
        aRec.setValue("Salary", aRec.value("Salary").toFloat()*1.1);
        tableModel->setRecord(i,aRec);
    }

    if(tableModel->submitAll())
        QMessageBox::information(this,"消息","涨工资成功！");
}


void MainWindow::on_actSubmit_triggered()
{
    bool res=tableModel->submitAll();
    if(!res)
        QMessageBox::information(this,"提示","数据库保存失败\n"+ tableModel->lastError().text());
    else{
        ui->actSubmit->setEnabled(false);
        ui->actRevert->setEnabled(false);
    }

    UpdateStatusbar();
}


void MainWindow::on_actRevert_triggered()
{
    tableModel->revertAll();
    ui->actSubmit->setEnabled(false);
    ui->actRevert->setEnabled(false);
    UpdateStatusbar();
}


void MainWindow::on_comboFields_currentIndexChanged(int index)
{
    if(ui->radioBtnDescend)
        tableModel->setSort(index,Qt::AscendingOrder);
    else
        tableModel->setSort(index, Qt::DescendingOrder);

    tableModel->select();
}


void MainWindow::on_radioBtnAscend_clicked()
{
    tableModel->sort(ui->comboFields->currentIndex(),Qt::AscendingOrder);
}


void MainWindow::on_radioBtnDescend_clicked()
{
    tableModel->sort(ui->comboFields->currentIndex(),Qt::DescendingOrder);
}


void MainWindow::on_radioBtnMan_clicked()
{
    tableModel->setFilter("Gender='男'");
    UpdateStatusbar();
}


void MainWindow::on_radioBtnWoman_clicked()
{
    tableModel->setFilter("Gender='女'");
    UpdateStatusbar();
}


void MainWindow::on_radioBtnBoth_clicked()
{
    tableModel->setFilter("");
    UpdateStatusbar();
}




void MainWindow::on_actRecFirst_triggered()
{
    dataMapper->toFirst();
    RefreshTableView();
}


void MainWindow::on_actRecPrevious_triggered()
{
    dataMapper->toPrevious();
    RefreshTableView();
}


void MainWindow::on_actRecNext_triggered()
{
    dataMapper->toNext();
    RefreshTableView();
}


void MainWindow::on_actRecLast_triggered()
{
    dataMapper->toLast();
    RefreshTableView();
}

