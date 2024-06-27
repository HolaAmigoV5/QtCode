#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->sliderZoom->setRange(10,500);
    ui->sliderH->setRange(-180,180);
    ui->sliderV->setRange(-180,180);

    iniGraph3D();
    QSplitter *splitter=new QSplitter(Qt::Horizontal);
    splitter->addWidget(ui->frameSetup);
    splitter->addWidget(graphContainer);
    setCentralWidget(splitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::iniGraph3D(){
    graph3D=new Q3DScatter();
    graphContainer=QWidget::createWindowContainer(graph3D,this);

    series=new QScatter3DSeries();
    series->setItemLabelFormat("(@xLabel, @zLabel, @yLabel)");
    series->setMeshSmooth(true);
    series->setMesh(QAbstract3DSeries::MeshSphere);
    series->setItemSize(0.2);
    graph3D->addSeries(series);

    // 轴
    graph3D->axisX()->setTitle("axis X");
    graph3D->axisX()->setLabelFormat("%.2f");
    graph3D->axisX()->setTitleVisible(true);

    graph3D->axisY()->setTitle("axis Y");
    graph3D->axisY()->setLabelFormat("%.2f");
    graph3D->axisY()->setTitleVisible(true);

    graph3D->axisZ()->setTitle("axis Z");
    graph3D->axisZ()->setLabelFormat("%.2f");
    graph3D->axisZ()->setTitleVisible(true);

    graph3D->activeTheme()->setLabelBackgroundEnabled(false);

    // 墨西哥草帽， -10 —— 10， 步长 0.5， 41步
    const int N=41;
    int itemCount=N*N;
    QScatterDataArray *dataArray=new QScatterDataArray();
    dataArray->resize(itemCount);

    QScatterDataItem *ptrToDataArray=&dataArray->first();
    float x,y,z;    // z=f(x,y)
    x=-10;
    for(int i=0; i<N; ++i){
        y=-10;
        for(int j=1; j<N; ++j){
            z=qSqrt(x*x+y*y);
            if(z!=0)
                z=10*qSin(z)/z;
            else
                z=10;

            ptrToDataArray->setPosition(QVector3D(x,z,y));
            ptrToDataArray++;
            y+=0.5;
        }
        x+=0.5;
    }

    series->dataProxy()->resetArray(dataArray);

    connect(series, &QScatter3DSeries::selectedItemChanged, this, &MainWindow::do_selectedItemChanged);
}

void MainWindow::do_selectedItemChanged(int index)
{
    ui->actPoint_ChangeValue->setEnabled(index>=0);
    ui->actData_Delete->setEnabled(index>=0);
    if(index>0){
        QScatterDataItem item=*(series->dataProxy()->itemAt(index));
        QString str=QString::asprintf("选中点的坐标 (x, z, y) = (%.2f, %.2f, %.2f)", item.x(), item.z(), item.y());
        ui->statusBar->showMessage(str);
    }
}


void MainWindow::on_actSeries_BaseColor_triggered()
{
    QColor color=series->baseColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        series->setBaseColor(color);
}

// 修改点坐标
void MainWindow::on_actPoint_ChangeValue_triggered()
{
    int index=series->selectedItem();
    if(index<0)
        return;

    QScatterDataItem item=*(series->dataProxy()->itemAt(index));
    QString coord=QString::asprintf("%.2f, %.2f, %.2f", item.x(), item.z(), item.y());
    bool ok=false;
    QString newText=QInputDialog::getText(this, "修改点坐标", "按格式输入点的坐标(x,z,y)", QLineEdit::Normal, coord,&ok).simplified();
    if(ok){
        QStringList xzy=newText.split(QLatin1Char(','),Qt::SkipEmptyParts); // 按照逗号分隔
        if(xzy.size()!=3){
            QMessageBox::critical(this,"错误","输入坐标数据格式错误");
            return;
        }

        item.setX(xzy[0].toFloat());
        item.setZ(xzy[1].toFloat());
        item.setY(xzy[2].toFloat());

        series->dataProxy()->setItem(index, item);
    }

}


void MainWindow::on_actData_Add_triggered()
{
    int x=QRandomGenerator::global()->bounded(-10,10);
    int z=QRandomGenerator::global()->bounded(-10,10);
    int y=QRandomGenerator::global()->bounded(5,10);
    QScatterDataItem item;
    item.setX(x);
    item.setY(y);
    item.setZ(z);
    series->dataProxy()->addItem(item);
}


void MainWindow::on_actData_Delete_triggered()
{
    int index=series->selectedItem();
    if(index<0)
        return;

    series->dataProxy()->removeItems(index,1);
}


void MainWindow::on_comboCamera_currentIndexChanged(int index)
{
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPreset(index));
}


void MainWindow::on_sliderH_valueChanged(int value)
{
    graph3D->scene()->activeCamera()->setXRotation(value);
}


void MainWindow::on_sliderV_valueChanged(int value)
{
    graph3D->scene()->activeCamera()->setYRotation(value);
}


void MainWindow::on_sliderZoom_valueChanged(int value)
{
    graph3D->scene()->activeCamera()->setZoomLevel(value);
}


void MainWindow::on_btnResetCamera_clicked()
{
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);
}


void MainWindow::on_btnMoveUp_clicked()
{
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_btnMoveDown_clicked()
{
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal z=target3D.z();
    target3D.setZ(z+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_btnMoveLeft_clicked()
{
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal x=target3D.x();
    target3D.setX(x+0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_btnMoveRight_clicked()
{
    QVector3D target3D=graph3D->scene()->activeCamera()->target();
    qreal x=target3D.x();
    target3D.setX(x-0.1);
    graph3D->scene()->activeCamera()->setTarget(target3D);
}


void MainWindow::on_cBoxTheme_currentIndexChanged(int index)
{
    Q3DTheme *curTheme=graph3D->activeTheme();
    curTheme->setType(Q3DTheme::Theme(index));
}


void MainWindow::on_spinFontSize_valueChanged(int arg1)
{
    QFont font=graph3D->activeTheme()->font();
    font.setPointSize(arg1);
    graph3D->activeTheme()->setFont(font);
}


void MainWindow::on_cBoxSelectionMode_currentIndexChanged(int index)
{
    graph3D->setSelectionMode(QAbstract3DGraph::SelectionFlags(index));
}


void MainWindow::on_chkBoxBackground_clicked(bool checked)
{
    graph3D->activeTheme()->setBackgroundEnabled(checked);
}


void MainWindow::on_chkBoxGrid_clicked(bool checked)
{
    graph3D->activeTheme()->setGridEnabled(checked);
}


void MainWindow::on_chkBoxShadow_clicked(bool checked)
{
    if(checked)
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityMedium);
    else
        graph3D->setShadowQuality(QAbstract3DGraph::ShadowQualityNone);
}


void MainWindow::on_chkBoxReverse_clicked(bool checked)
{
    graph3D->axisY()->setReversed(checked);
}


void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
{
    graph3D->axisX()->setTitleVisible(checked);
    graph3D->axisY()->setTitleVisible(checked);
    graph3D->axisZ()->setTitleVisible(checked);
}


void MainWindow::on_chkBoxAxisBackground_clicked(bool checked)
{
    graph3D->activeTheme()->setLabelBackgroundEnabled(checked);
}


void MainWindow::on_cBoxBarStyle_currentIndexChanged(int index)
{
     series->setMesh(QAbstract3DSeries::Mesh(index+1));
}


void MainWindow::on_spinItemSize_valueChanged(double arg1)
{
    series->setItemSize(arg1);
}


void MainWindow::on_chkBoxSmooth_clicked(bool checked)
{
    series->setMeshSmooth(checked);
}


void MainWindow::on_chkBoxItemLabel_clicked(bool checked)
{
    series->setItemLabelVisible(checked);
}

