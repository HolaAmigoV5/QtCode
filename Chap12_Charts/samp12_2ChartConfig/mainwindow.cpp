﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRandomGenerator>
#include <QMargins>
#include <QColorDialog>
#include <QFontDialog>
#include "tpendialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(ui->splitter);

    createChart();
    prepareData();
    updateFromChart();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createChart()
{
    chart=new QChart();
    chart->setTitle("我的图表");
    ui->chartView->setChart(chart);
    ui->chartView->setRenderHint(QPainter::Antialiasing);

    QLineSeries *series0=new QLineSeries();
    QLineSeries *series1=new QLineSeries();
    series0->setName("Sin曲线");
    series1->setName("Cos曲线");

    curSeries=series0;

    QPen pen;
    pen.setStyle(Qt::DotLine);
    pen.setWidth(2);
    pen.setColor(Qt::red);
    series0->setPen(pen);

    pen.setStyle(Qt::SolidLine);
    pen.setColor(Qt::blue);
    series1->setPen(pen);

    chart->addSeries(series0);
    chart->addSeries(series1);

    QValueAxis *axisX=new QValueAxis();
    curAxis=axisX;
    axisX->setRange(0,10);
    axisX->setLabelFormat("%.1f");
    axisX->setTickCount(11);
    axisX->setTitleText("time(secs)");

    QValueAxis *axisY=new QValueAxis();
    axisY->setRange(-2,2);
    axisY->setLabelFormat("%.2f");
    axisY->setTickCount(5);
    axisY->setMinorTickCount(2);
    axisY->setTitleText("value");

    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);

    series0->attachAxis(axisX);
    series0->attachAxis(axisY);

    series1->attachAxis(axisX);
    series1->attachAxis(axisY);

}


void MainWindow::prepareData()
{
    QLineSeries *series0=static_cast<QLineSeries*>(chart->series().at(0));
    QLineSeries *series1=static_cast<QLineSeries*>(chart->series().at(1));

    series0->clear();
    series1->clear();

    qreal x=0, intv=0.1;
    for (int i = 0; i < 100; ++i) {
        int rd=QRandomGenerator::global()->bounded(-5,6);   // [-5,6)
        series0->append(x, qSin(x)+rd/50.0);
        rd=QRandomGenerator::global()->bounded(-5,6);   // [-5,6)
        series1->append(x, qCos(x)+rd/50.0);
        x+=intv;
    }
}

void MainWindow::updateFromChart()
{
    QChart *curChart=ui->chartView->chart();
    ui->editTitle->setText(curChart->title());

    QMargins mg=curChart->margins();
    ui->spinMarginLeft->setValue(mg.left());
    ui->spinMarginRight->setValue(mg.right());
    ui->spinMarginTop->setValue(mg.top());
    ui->spinMarginBottom->setValue(mg.bottom());
}


// 刷新绘图
void MainWindow::on_actDraw_triggered()
{
    prepareData();
}

// 设置标题
void MainWindow::on_btnTitleSetText_clicked()
{
    chart->setTitle(ui->editTitle->text().trimmed());
}

// 标题文字颜色
void MainWindow::on_btnTitleColor_clicked()
{
    QColor color=chart->titleBrush().color();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        chart->setTitleBrush(QBrush(color));
}

// 标题字体
void MainWindow::on_btnTitleFont_clicked()
{
    QFont font=chart->titleFont();
    bool ok=false;
    font=QFontDialog::getFont(&ok, font);
    if(ok)
        chart->setTitleFont(font);
}

// 图例
void MainWindow::on_groupBox_Legend_clicked(bool checked)
{
    chart->legend()->setVisible(checked);
}


void MainWindow::on_radioButton_clicked()
{
    chart->legend()->setAlignment(Qt::AlignTop);
}


void MainWindow::on_radioButton_2_clicked()
{
    chart->legend()->setAlignment(Qt::AlignBottom);
}


void MainWindow::on_radioButton_3_clicked()
{
    chart->legend()->setAlignment(Qt::AlignLeft);
}


void MainWindow::on_radioButton_4_clicked()
{
    chart->legend()->setAlignment(Qt::AlignRight);
}

// 图例文字颜色
void MainWindow::on_btnLegendlabelColor_clicked()
{
    QColor color=chart->legend()->labelColor();

    color=QColorDialog::getColor(color);
    if(color.isValid())
        chart->legend()->setLabelColor(color);
}

// 图例字体
void MainWindow::on_btnLegendFont_clicked()
{
    QFont font=chart->legend()->font();
    bool ok=false;
    font= QFontDialog::getFont(&ok,font);
    if(ok)
        chart->legend()->setFont(font);
}

// 设置边距
void MainWindow::on_btnSetMargin_clicked()
{
    QMargins mg;
    mg.setLeft(ui->spinMarginLeft->value());
    mg.setRight(ui->spinMarginRight->value());
    mg.setTop(ui->spinMarginTop->value());
    mg.setBottom(ui->spinMarginBottom->value());
    chart->setMargins(mg);
}

// 动画
void MainWindow::on_comboAnimation_currentIndexChanged(int index)
{
    chart->setAnimationOptions(QChart::AnimationOptions(index));
    prepareData();
}

// 主题
void MainWindow::on_comboTheme_currentIndexChanged(int index)
{
    chart->setTheme(QChart::ChartTheme(index));
}

// *********************************************************************************************
// 曲线设置
void MainWindow::on_radioSeries0_clicked()
{
    if(ui->radioSeries0->isChecked())
        curSeries=static_cast<QLineSeries*>(chart->series().at(0));
    else
        curSeries=static_cast<QLineSeries*>(chart->series().at(1));

    ui->editSeriesName->setText(curSeries->name());
    ui->groupBox_Series->setChecked(curSeries->isVisible());
    ui->groupBox_Points->setChecked(curSeries->pointsVisible());
    ui->chkkBoxUseOpenGL->setChecked(curSeries->useOpenGL());
    ui->sliderOpacity->setValue(curSeries->opacity()*10);
    ui->groupBox_PointLabel->setChecked(curSeries->pointLabelsVisible());
}


void MainWindow::on_radioSeries1_clicked()
{
    on_radioSeries0_clicked();
}


void MainWindow::on_groupBox_Series_clicked(bool checked)
{
    curSeries->setVisible(checked);
}

// 序列名称
void MainWindow::on_btnSeriesName_clicked()
{
    curSeries->setName(ui->editSeriesName->text());
    if(ui->radioSeries0->isChecked())
        ui->radioSeries0->setText(ui->editSeriesName->text());
    else
        ui->radioSeries1->setText(ui->editSeriesName->text());
}


void MainWindow::on_btnSeriesColor_clicked()
{
    QColor color=curSeries->color();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curSeries->setColor(color);
}


void MainWindow::on_btnSeriesPen_clicked()
{
    QPen pen=curSeries->pen();
    bool ok=false;
    pen=TPenDialog::getPen(pen, &ok);
    if(ok)
        curSeries->setPen(pen);
}


void MainWindow::on_chkkBoxUseOpenGL_clicked(bool checked)
{
    curSeries->setUseOpenGL(checked);
}



void MainWindow::on_sliderOpacity_sliderMoved(int position)
{
    curSeries->setOpacity(position/10.0);
}


void MainWindow::on_groupBox_Points_clicked(bool checked)
{
    curSeries->setPointsVisible(checked);
}


void MainWindow::on_doubleSpinBox_valueChanged(double arg1)
{
    curSeries->setMarkerSize(arg1);
}


void MainWindow::on_btnPointColor_clicked()
{
    QColor color=QColorDialog::getColor();
    if(color.isValid())
        curSeries->setBrush(QBrush(color));
}


void MainWindow::on_groupBox_PointLabel_clicked(bool checked)
{
    curSeries->setPointLabelsVisible(checked);
}


void MainWindow::on_btnSeriesLabColor_clicked()
{
    QColor color=curSeries->pointLabelsColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curSeries->setPointLabelsColor(color);
}


void MainWindow::on_btnSeriesLabFont_clicked()
{
    QFont font=curSeries->pointLabelsFont();
    bool ok=false;
    font=QFontDialog::getFont(&ok, font);
    if(ok)
        curSeries->setPointLabelsFont(font);
}


void MainWindow::on_radioSeriesLabFormat0_clicked()
{
    curSeries->setPointLabelsFormat("@yPoint");
}


void MainWindow::on_radioSeriesLabFormat1_clicked()
{
    curSeries->setPointLabelsFormat("(@xPoint, @yPoint)");
}


void MainWindow::on_radioAxisX_clicked()
{
    if(ui->radioAxisX->isChecked())
        curAxis=static_cast<QValueAxis*>(chart->axes(Qt::Horizontal).at(0));
    else
        curAxis=static_cast<QValueAxis*>(chart->axes(Qt::Vertical).at(0));

    // 更新界面
    ui->groupBox_Axis->setChecked(curAxis->isVisible());
    ui->chkBoxAxisReverse->setChecked(curAxis->isReverse());
    ui->spinAxisMax->setValue(curAxis->max());
    ui->spinAxisMin->setValue(curAxis->min());

    ui->groupBox_AxisTitle->setChecked(curAxis->isTitleVisible());
    ui->editAxisTitle->setText(curAxis->titleText());

    ui->groupBox_AxisLabel->setChecked(curAxis->labelsVisible());
    ui->editAxisLabelFormat->setText(curAxis->labelFormat());

    ui->groupBox_Ticks->setChecked(curAxis->isLineVisible());
    if(curAxis->tickType()==QValueAxis::TicksFixed)
        ui->radioTick_Fixed->setChecked(true);
    else
        ui->radioTick_Dynamic->setChecked(true);

    ui->spinTick_Anchor->setValue(curAxis->tickAnchor());   // 动态刻度起点
    ui->spinTick_Intv->setValue(curAxis->tickInterval());   // 动态刻度间隔
    ui->spinTick_Count->setValue(curAxis->tickCount());

    ui->groupBox_GridLine->setChecked(curAxis->isGridLineVisible());
    ui->groupBox_MinorGrid->setChecked(curAxis->isMinorGridLineVisible());
    ui->spinMinorTickCount->setValue(curAxis->minorTickCount());
}


void MainWindow::on_radioAxisY_clicked()
{
    on_radioAxisX_clicked();
}


void MainWindow::on_groupBox_Axis_clicked(bool checked)
{
    curAxis->setVisible(checked);
}


void MainWindow::on_chkBoxAxisReverse_clicked(bool checked)
{
    curAxis->setReverse(checked);
}


void MainWindow::on_btnSetAxisRange_clicked()
{
    curAxis->setRange(ui->spinAxisMin->value(),ui->spinAxisMax->value());
}


void MainWindow::on_groupBox_AxisTitle_clicked(bool checked)
{
    curAxis->setTitleVisible(checked);
}


void MainWindow::on_btnAxisSetTitle_clicked()
{
    curAxis->setTitleText(ui->editAxisTitle->text());
}


void MainWindow::on_btnAxisTitleColor_clicked()
{
    QColor color=curAxis->titleBrush().color();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curAxis->setTitleBrush(QBrush(color));
}


void MainWindow::on_btnAxisTitleFont_clicked()
{
    QFont font=curAxis->titleFont();
    bool ok=false;
    font=QFontDialog::getFont(&ok,font);
    if(ok)
        curAxis->setTitleFont(font);
}


void MainWindow::on_groupBox_AxisLabel_clicked(bool checked)
{
    curAxis->setLabelsVisible(checked);
}


void MainWindow::on_btnAxisLabelFormat_clicked()
{
    curAxis->setLabelFormat(ui->editAxisLabelFormat->text());
}


void MainWindow::on_btnAxisLabelColor_clicked()
{
    QColor color=curAxis->labelsColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curAxis->setLabelsColor(color);
}


void MainWindow::on_btnAxisLabelFont_clicked()
{
    QFont font=curAxis->labelsFont();
    bool ok=false;
    font=QFontDialog::getFont(&ok,font);
    if(ok)
        curAxis->setLabelsFont(font);
}


void MainWindow::on_groupBox_Ticks_clicked(bool checked)
{
    curAxis->setLineVisible(checked);
}


void MainWindow::on_radioTick_Fixed_clicked()
{
    curAxis->setTickType(QValueAxis::TicksFixed);

}


void MainWindow::on_radioTick_Dynamic_clicked()
{
    curAxis->setTickType(QValueAxis::TicksDynamic);
}


void MainWindow::on_radioTick_Fixed_toggled(bool checked)
{
    if(checked)
    {
        ui->spinTick_Count->setEnabled(true);
        ui->spinTick_Anchor->setEnabled(false);
        ui->spinTick_Intv->setEnabled(false);
    }
    else{
        ui->spinTick_Count->setEnabled(false);
        ui->spinTick_Anchor->setEnabled(true);
        ui->spinTick_Intv->setEnabled(true);
    }
}


void MainWindow::on_spinTick_Count_valueChanged(int arg1)
{
    curAxis->setTickCount(arg1);
}


void MainWindow::on_spinTick_Anchor_valueChanged(double arg1)
{
    curAxis->setTickAnchor(arg1);
}


void MainWindow::on_spinTick_Intv_valueChanged(double arg1)
{
    curAxis->setTickInterval(arg1);
}


void MainWindow::on_btnAxisLinePen_clicked()
{
    QPen pen=curAxis->linePen();
    bool ok=false;
    pen=TPenDialog::getPen(pen,&ok);
    if(ok)
        curAxis->setLinePen(pen);
}


void MainWindow::on_btnAxisLinePenColor_clicked()
{
    QColor color=curAxis->linePenColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curAxis->setLinePenColor(color);
}


void MainWindow::on_groupBox_GridLine_clicked(bool checked)
{
    curAxis->setGridLineVisible(checked);
}


void MainWindow::on_btnGridLinePen_clicked()
{
    QPen pen=curAxis->gridLinePen();
    bool ok=false;
    pen=TPenDialog::getPen(pen,&ok);
    if(ok)
        curAxis->setGridLinePen(pen);
}


void MainWindow::on_btnGridLineColor_clicked()
{
    QColor color=curAxis->gridLineColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curAxis->setGridLineColor(color);
}


void MainWindow::on_groupBox_MinorGrid_clicked(bool checked)
{
    curAxis->setMinorGridLineVisible(checked);
}


void MainWindow::on_spinMinorTickCount_valueChanged(int arg1)
{
    curAxis->setMinorTickCount(ui->spinMinorTickCount->value());
}


void MainWindow::on_btnMinorPen_clicked()
{
    QPen pen=curAxis->minorGridLinePen();
    bool ok=false;
    pen=TPenDialog::getPen(pen,&ok);
    if(ok)
        curAxis->setMinorGridLinePen(pen);
}


void MainWindow::on_btnMinorColor_clicked()
{
    QColor color=curAxis->minorGridLineColor();
    color=QColorDialog::getColor(color);
    if(color.isValid())
        curAxis->setMinorGridLineColor(color);
}

