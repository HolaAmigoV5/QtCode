#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSplitter>
#include <QColorDialog>
#include <QInputDialog>

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

void MainWindow::iniGraph3D()
{
    graph3D=new Q3DBars();
    graphContainer=QWidget::createWindowContainer(graph3D,this);
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);

    // 轴 value=(row, col)
    QValue3DAxis *axisV=new QValue3DAxis;
    axisV->setTitle("销量");
    axisV->setTitleVisible(true);
    axisV->setLabelFormat("%d");
    graph3D->setValueAxis(axisV);

    QCategory3DAxis *axisRow=new QCategory3DAxis;
    axisRow->setTitle("周");
    axisRow->setTitleVisible(true);
    graph3D->setRowAxis(axisRow);

    QCategory3DAxis *axisCol=new QCategory3DAxis;
    axisCol->setTitle("星期");
    axisCol->setTitleVisible(true);
    graph3D->setColumnAxis(axisCol);

    // 序列
    series=new QBar3DSeries;
    series->setMesh(QAbstract3DSeries::MeshCylinder);
    series->setItemLabelFormat("(@rowLabel, @colLabel): %d");
    graph3D->addSeries(series);

    // 代理
    QBarDataArray *dataArray=new QBarDataArray;
    for(int i=0; i<4; i++){
        QBarDataRow *dataRow=new QBarDataRow;
        for(int j=1; j<6; j++){
            quint32 value=QRandomGenerator::global()->bounded(5,15);
            QBarDataItem *dataItem=new QBarDataItem;
            dataItem->setValue(value);
            dataRow->append(*dataItem);
        }
        dataArray->append(dataRow);
    }

    QStringList rowLabs;
    rowLabs<<"Week1"<<"Week2"<<"Week3"<<"Week4";
    series->dataProxy()->setRowLabels(rowLabs);

    QStringList colLabs;
    colLabs<<"Mon"<<"Tue"<<"Wed"<<"Thur"<<"Fri";
    series->dataProxy()->setColumnLabels(colLabs);

    series->dataProxy()->resetArray(dataArray);

    connect(series, &QBar3DSeries::selectedBarChanged, this, &MainWindow::do_selectedBarChanged);
}


void MainWindow::do_selectedBarChanged(const QPoint &position)
{
    if(position.x()<0 || position.y()<0){
        ui->actBar_ChangeValue->setEnabled(false);
        return;
    }

    const QBarDataItem *bar=series->dataProxy()->itemAt(position);
    ui->actBar_ChangeValue->setEnabled(true);
    QString info=QString::asprintf("选中的Bar是， Row=%d, Colum=%d, Value=%.1f",position.x(),position.y(),bar->value());
    ui->statusBar->showMessage(info);
}

void MainWindow::on_actSeries_BaseColor_triggered()
{
    QColor color=series->baseColor();
    color=QColorDialog::getColor(color,this,"选择颜色");
    if(color.isValid())
        series->setBaseColor(color);
}


void MainWindow::on_actRedraw_triggered()
{
    // 代理
    QBarDataArray *dataArray=new QBarDataArray;
    for(int i=0; i<4; i++){
        QBarDataRow *dataRow=new QBarDataRow;
        for(int j=1; j<6; j++){
            quint32 value=QRandomGenerator::global()->bounded(5,15);
            QBarDataItem *dataItem=new QBarDataItem;
            dataItem->setValue(value);
            dataRow->append(*dataItem);
        }
        dataArray->append(dataRow);
    }
    series->dataProxy()->resetArray(dataArray);
}

void MainWindow::on_actBar_ChangeValue_triggered()
{
    QPoint position=series->selectedBar();
    if(position.x()<0 || position.y()<0)
        return;

    QBarDataItem barItem(*(series->dataProxy()->itemAt(position)));
    qreal value=barItem.value();
    bool ok=false;
    value=QInputDialog::getInt(this, "输入数值", "更改柱体的值",value,0,50,1,&ok);
    if(ok){
        barItem.setValue(value);
        series->dataProxy()->setItem(position,barItem);
    }
}


void MainWindow::on_actData_Add_triggered()
{
    QString rowLabel=QInputDialog::getText(this,"输入字符串","请输入行标签");
    if(rowLabel.isEmpty())
        return;

    QBarDataRow *dataRow=new QBarDataRow;
    for(int j=1; j<6; j++){
        quint32 value=QRandomGenerator::global()->bounded(5,15);
        QBarDataItem *dataItem=new QBarDataItem;
        dataItem->setValue(value);
        dataRow->append(*dataItem);
    }
    series->dataProxy()->addRow(dataRow,rowLabel);
}


void MainWindow::on_actData_Insert_triggered()
{
    QString rowLabel=QInputDialog::getText(this,"输入字符串","请输入行标签");
    if(rowLabel.isEmpty())
        return;

    QPoint position=series->selectedBar();
    int index=position.x();
    if(index<0)
        index=0;

    QBarDataRow *dataRow=new QBarDataRow;
    for(int j=1; j<6; j++){
        quint32 value=QRandomGenerator::global()->bounded(5,15);
        QBarDataItem *dataItem=new QBarDataItem;
        dataItem->setValue(value);
        dataRow->append(*dataItem);
    }
    series->dataProxy()->insertRow(index,dataRow,rowLabel);
}


void MainWindow::on_actData_Delete_triggered()
{
    QPoint position=series->selectedBar();
    if(position.x()<0|| position.y()<0)
        return;

    series->dataProxy()->removeRows(position.x(),1);
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


void MainWindow::on_btnResetCamera_pressed()
{
    graph3D->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFrontHigh);

}


void MainWindow::on_btnMoveUp_pressed()
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


void MainWindow::on_chkBoxReflection_clicked(bool checked)
{
    graph3D->setReflection(checked);
}


void MainWindow::on_chkBoxReverse_clicked(bool checked)
{
    graph3D->valueAxis()->setReversed(checked);
}


void MainWindow::on_chkBoxAxisTitle_clicked(bool checked)
{
    graph3D->valueAxis()->setTitleVisible(checked);
    graph3D->rowAxis()->setTitleVisible(checked);
    graph3D->columnAxis()->setTitleVisible(checked);
}


void MainWindow::on_chkBoxAxisBackground_clicked(bool checked)
{
    graph3D->activeTheme()->setLabelBackgroundEnabled(checked);
}


void MainWindow::on_cBoxBarStyle_currentIndexChanged(int index)
{
    series->setMesh(QAbstract3DSeries::Mesh(index+1));
}


void MainWindow::on_chkBoxSmooth_clicked(bool checked)
{
    series->setMeshSmooth(checked);
}


void MainWindow::on_chkBoxItemLabel_clicked(bool checked)
{
    series->setItemLabelVisible(checked);
}

