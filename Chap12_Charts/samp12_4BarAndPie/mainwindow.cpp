#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tableView->setAlternatingRowColors(true);
    ui->treeWidget->setAlternatingRowColors(true);
    studCount=ui->spinCount->value();
    dataModel=new QStandardItemModel(this);
    ui->tableView->setModel(dataModel);

    generateData();
    countData();

    iniBarChart();
    iniPercentBar();
    iniPieChart();
    iniStackedBar();

    // 防治拖尾
    ui->chartViewBar->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->chartViewPercentBar->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->chartViewStackedBar->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    ui->chartViewPie->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::generateData()
{
    QStringList headerList;
    headerList<<"姓名"<<"数学"<<"语文"<<"英语"<<"平均分";
    dataModel->setHorizontalHeaderLabels(headerList);

    QList<QStandardItem*> itemList;  // 一行的数据
    QStandardItem *item;
    for (int i = 0; i < studCount; ++i) {
        itemList.clear();
        QString str=QString::asprintf("学生%2d", i+1);
        item=new QStandardItem(str);
        item->setTextAlignment(Qt::AlignCenter);
        itemList<<item;

        qreal sumScore=0;
        for (int j = COL_MATH; j <= COL_ENGLISH; ++j) {
            qreal score=50.0+QRandomGenerator::global()->bounded(0,50);
            sumScore+=score;
            str=QString::asprintf("%.0f", score);
            item=new QStandardItem(str);
            item->setTextAlignment(Qt::AlignCenter);
            itemList<<item;
        }

        str=QString::asprintf("%.1f", sumScore/3);
        item=new QStandardItem(str);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        itemList<<item;

        dataModel->appendRow(itemList);

    }
}

void MainWindow::countData()
{
    QTreeWidgetItem *item;
    for (int i = COL_MATH; i <= COL_ENGLISH; ++i) {
        int cnt50=0;
        int cnt60=0;
        int cnt70=0;
        int cnt80=0;
        int cnt90=0;
        for (int j = 0; j < dataModel->rowCount(); ++j) {
            qreal core=dataModel->item(j,i)->text().toDouble();
            if(core<60)
                cnt50++;
            else if(core<70 && core>=60)
                cnt60++;
            else if(core<80 && core>=70)
                cnt70++;
            else if(core<90 && core>=80)
                cnt80++;
            else
                cnt90++;
        }

        item=ui->treeWidget->topLevelItem(0);
        item->setText(i,QString::number(cnt50));
        item->setTextAlignment(i,Qt::AlignCenter);

        item=ui->treeWidget->topLevelItem(1);
        item->setText(i,QString::number(cnt60));
        item->setTextAlignment(i,Qt::AlignCenter);

        item=ui->treeWidget->topLevelItem(2);
        item->setText(i,QString::number(cnt70));
        item->setTextAlignment(i,Qt::AlignCenter);

        item=ui->treeWidget->topLevelItem(3);
        item->setText(i,QString::number(cnt80));
        item->setTextAlignment(i,Qt::AlignCenter);

        item=ui->treeWidget->topLevelItem(4);
        item->setText(i,QString::number(cnt90));
        item->setTextAlignment(i,Qt::AlignCenter);
    }
}

void MainWindow::removeAllAxis(QChart *chart)
{
    foreach (auto theAxis, chart->axes()) {
        chart->removeAxis(theAxis);
        delete theAxis;
    }
}

void MainWindow::iniBarChart()
{
    QChart *chart=new QChart();
    chart->setTitle("BarChart 演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewBar->setChart(chart);
    ui->chartViewBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawBarChart(bool isVertical)
{
    QChart *chart=ui->chartViewBar->chart();
    chart->removeAllSeries();
    removeAllAxis(chart);

    // 绘制坐标轴
    QValueAxis *axisValue=new QValueAxis();
    axisValue->setRange(0,100);
    axisValue->setTitleText("分数");
    axisValue->setLabelFormat("%.0f");
    axisValue->applyNiceNumbers();

    QBarCategoryAxis *axisStud=new QBarCategoryAxis();
    QStringList categories;
    for(int i=0; i<dataModel->rowCount();++i){
        categories<<dataModel->item(i,COL_NAME)->text();
    }
    axisStud->append(categories);

    if(isVertical){
        chart->addAxis(axisStud,Qt::AlignBottom);
        chart->addAxis(axisValue,Qt::AlignLeft);
    }
    else{
        chart->addAxis(axisStud,Qt::AlignLeft);
        chart->addAxis(axisValue,Qt::AlignBottom);
    }

    // 绘制Bar
    QBarSet *setMath=new QBarSet("数学");
    QBarSet *setChinese=new QBarSet("语文");
    QBarSet *setEnglish=new QBarSet("英语");

    QAbstractBarSeries *seriesBar;
    if(isVertical)
        seriesBar=new QBarSeries();
    else
        seriesBar=new QHorizontalBarSeries();

    seriesBar->append(setMath);
    seriesBar->append(setChinese);
    seriesBar->append(setEnglish);

    seriesBar->setLabelsVisible(true);
    seriesBar->setLabelsFormat("@value");

    chart->addSeries(seriesBar);
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);


    // 绘制平均分
    QLineSeries *seriesLine=new QLineSeries();
    seriesLine->setName("平均分");
    QPen pen(Qt::red);
    pen.setWidth(2);
    seriesLine->setPen(pen);

    QFont font=seriesLine->pointLabelsFont();
    font.setPointSize(12);
    font.setBold(true);
    seriesLine->setPointLabelsFont(font);
    seriesLine->setPointLabelsVisible(true);

    if(isVertical)
        seriesLine->setPointLabelsFormat("@yPoint");
    else
        seriesLine->setPointLabelsFormat("@xPoint");

    for (int i = 0; i < dataModel->rowCount(); ++i) {
        setMath->append(dataModel->item(i,COL_MATH)->text().toInt());
        setChinese->append(dataModel->item(i,COL_CHINESE)->text().toInt());
        setEnglish->append(dataModel->item(i,COL_ENGLISH)->text().toInt());

        if(isVertical)
            seriesLine->append(i,dataModel->item(i,COL_AVERAGE)->text().toDouble());
        else
            seriesLine->append(dataModel->item(i,COL_AVERAGE)->text().toDouble(),i);
    }

    chart->addSeries(seriesLine);
    seriesLine->attachAxis(axisStud);
    seriesLine->attachAxis(axisValue);

    chart->legend()->setAlignment(Qt::AlignBottom);

    connect(seriesBar,&QBarSeries::hovered,this,&MainWindow::do_barHovered);
    connect(seriesBar,&QBarSeries::clicked,this,&MainWindow::do_barClicked);
}

void MainWindow::iniStackedBar()
{
    QChart *chart=new QChart();
    chart->setTitle("StackedBar 演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewStackedBar->setChart(chart);
    ui->chartViewStackedBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawStackedBar(bool isVertical)
{
    QChart *chart=ui->chartViewStackedBar->chart();
    chart->removeAllSeries();
    removeAllAxis(chart);

    // 绘制坐标轴
    QValueAxis *axisValue=new QValueAxis();
    axisValue->setRange(0,300);
    axisValue->setTitleText("分数");
    axisValue->setTickCount(7);
    axisValue->setLabelFormat("%.0f");
    axisValue->applyNiceNumbers();

    QBarCategoryAxis *axisStud=new QBarCategoryAxis();
    QStringList categories;
    for(int i=0; i<dataModel->rowCount();++i){
        categories<<dataModel->item(i,COL_NAME)->text();
    }
    axisStud->append(categories);

    if(isVertical){
        chart->addAxis(axisStud,Qt::AlignBottom);
        chart->addAxis(axisValue,Qt::AlignLeft);
    }
    else{
        chart->addAxis(axisStud,Qt::AlignLeft);
        chart->addAxis(axisValue,Qt::AlignBottom);
    }

    // 绘制Bar
    QBarSet *setMath=new QBarSet("数学");
    QBarSet *setChinese=new QBarSet("语文");
    QBarSet *setEnglish=new QBarSet("英语");

    QAbstractBarSeries *seriesBar;
    if(isVertical)
        seriesBar=new QStackedBarSeries();
    else
        seriesBar=new QHorizontalStackedBarSeries();

    seriesBar->append(setMath);
    seriesBar->append(setChinese);
    seriesBar->append(setEnglish);

    seriesBar->setLabelsVisible(true);
    seriesBar->setLabelsFormat("@value");

    chart->addSeries(seriesBar);
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);


    for (int i = 0; i < dataModel->rowCount(); ++i) {
        setMath->append(dataModel->item(i,COL_MATH)->text().toInt());
        setChinese->append(dataModel->item(i,COL_CHINESE)->text().toInt());
        setEnglish->append(dataModel->item(i,COL_ENGLISH)->text().toInt());
    }

    chart->legend()->setAlignment(Qt::AlignRight);

    connect(seriesBar,&QBarSeries::hovered,this,&MainWindow::do_barHovered);
    connect(seriesBar,&QBarSeries::clicked,this,&MainWindow::do_barClicked);
}

void MainWindow::iniPercentBar()
{
    QChart *chart=new QChart();
    chart->setTitle("PercentBar 演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewPercentBar->setChart(chart);
    ui->chartViewPercentBar->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawPercentBar(bool isVertical)
{
    QChart *chart=ui->chartViewPercentBar->chart();
    chart->removeAllSeries();
    removeAllAxis(chart);

    // 绘制坐标轴
    QValueAxis *axisValue=new QValueAxis();
    axisValue->setRange(0,100);
    axisValue->setTitleText("百分比");
    axisValue->setTickCount(6);
    axisValue->setLabelFormat("%.0f%%");
    axisValue->applyNiceNumbers();

    QBarCategoryAxis *axisStud=new QBarCategoryAxis();
    QStringList categories;
    categories<<"数学"<<"语文"<<"英语";
    axisStud->append(categories);

    if(isVertical){
        chart->addAxis(axisStud,Qt::AlignBottom);
        chart->addAxis(axisValue,Qt::AlignLeft);
    }
    else{
        chart->addAxis(axisStud,Qt::AlignLeft);
        chart->addAxis(axisValue,Qt::AlignBottom);
    }

    // 绘制Bar
    QAbstractBarSeries *seriesBar;
    if(isVertical)
        seriesBar=new QPercentBarSeries();
    else
        seriesBar=new QHorizontalPercentBarSeries();


    seriesBar->setLabelsVisible(true);
    seriesBar->setLabelsFormat("@value");

    chart->addSeries(seriesBar);
    seriesBar->attachAxis(axisStud);
    seriesBar->attachAxis(axisValue);

    QList<QBarSet*> barSetList;
    for (int i = 0; i < 5; ++i) {
        QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
        QBarSet *barSet=new QBarSet(item->text(0));
        barSet->append(item->text(1).toInt());  // 数学
        barSet->append(item->text(2).toInt());  // 语文
        barSet->append(item->text(3).toInt());  // 英语

        barSetList.append(barSet);
    }
    seriesBar->append(barSetList);
    chart->legend()->setAlignment(Qt::AlignRight);

    connect(seriesBar,&QBarSeries::hovered,this,&MainWindow::do_barHovered);
    connect(seriesBar,&QBarSeries::clicked,this,&MainWindow::do_barClicked);
}

void MainWindow::iniPieChart()
{
    QChart *chart=new QChart();
    chart->setTitle("PieChart 演示");
    chart->setAnimationOptions(QChart::SeriesAnimations);
    ui->chartViewPie->setChart(chart);
    ui->chartViewPie->setRenderHint(QPainter::Antialiasing);
}

void MainWindow::drawPieChart()
{
    QChart *chart=ui->chartViewPie->chart();
    chart->removeAllSeries();
    removeAllAxis(chart);

    int colNum=ui->comboCourse->currentIndex()+1;
    QPieSeries *seriesPie=new QPieSeries();
    seriesPie->setHoleSize(ui->spinHoleSize->value());
    for (int i = 0; i < 5; ++i) {
        QTreeWidgetItem *item=ui->treeWidget->topLevelItem(i);
        seriesPie->append(item->text(0),item->text(colNum).toDouble());
    }
    seriesPie->setLabelsVisible(true);

    QPieSlice *slice;
    for (int i = 0; i < 5; ++i) {
        slice=seriesPie->slices().at(i);
        slice->setLabel(slice->label()+QString::asprintf("：%.0f人, 占比 %.1f%%",slice->value(),slice->percentage()*100));
    }
    slice->setExploded(true);
    chart->setAcceptHoverEvents(true);
    chart->addSeries(seriesPie);
    chart->setTitle("PieChart---"+ui->comboCourse->currentText());
    chart->legend()->setAlignment(Qt::AlignRight);

    connect(seriesPie,&QPieSeries::hovered,this,&MainWindow::do_pieHovered);

}



void MainWindow::on_btnBuildBarChart_clicked()
{
    drawBarChart();
}


void MainWindow::on_btnBuildBarChartH_clicked()
{
    drawBarChart(false);
}


void MainWindow::on_btnBuildStackedBar_clicked()
{
    drawStackedBar();
}


void MainWindow::on_btnBuildStackedBarH_clicked()
{
    drawStackedBar(false);
}


void MainWindow::on_btnPercentBar_clicked()
{
    drawPercentBar();
}


void MainWindow::on_btnPercentBarH_clicked()
{
    drawPercentBar(false);
}


void MainWindow::on_btnDrawPieChart_clicked()
{
    drawPieChart();
}

void MainWindow::do_barHovered(bool status, int index, QBarSet *barset)
{
    QString str="hovered barSet="+barset->label();
    if(status)
        str+=QString::asprintf(", index=%d, value=%.2f",index, barset->at(index));
    else
        str="";

    ui->statusBar->showMessage(str);
}

void MainWindow::do_barClicked(int index, QBarSet *barset)
{
    QString str="clicked barSet="+barset->label();
    str+=QString::asprintf(", index=%d, count=%d",index, barset->count());
    ui->statusBar->showMessage(str);
}

void MainWindow::do_pieHovered(QPieSlice *slice, bool state)
{
    slice->setExploded(state);
}


void MainWindow::on_chkBox_PieLegend_clicked(bool checked)
{
    ui->chartViewPie->chart()->legend()->setVisible(checked);
}


void MainWindow::on_toolBtn_GenData_clicked()
{
    studCount=ui->spinCount->value();
    dataModel->clear();
    generateData();
}


void MainWindow::on_toolBtn_Counting_clicked()
{
    countData();
}


void MainWindow::on_comboTheme_currentIndexChanged(int index)
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        ui->chartViewBar->chart()->setTheme(QChart::ChartTheme(index));
        break;
    case 1:
        ui->chartViewStackedBar->chart()->setTheme(QChart::ChartTheme(index));
        break;
    case 2:
        ui->chartViewPercentBar->chart()->setTheme(QChart::ChartTheme(index));
        break;
    case 3:
        ui->chartViewPie->chart()->setTheme(QChart::ChartTheme(index));
        break;
    default:
        break;
    }
}


void MainWindow::on_comboAnimation_currentIndexChanged(int index)
{
    switch (ui->tabWidget->currentIndex()) {
    case 0:
        ui->chartViewBar->chart()->setAnimationOptions(QChart::AnimationOption(index));
        break;
    case 1:
        ui->chartViewStackedBar->chart()->setAnimationOptions(QChart::AnimationOption(index));
        break;
    case 2:
        ui->chartViewPercentBar->chart()->setAnimationOptions(QChart::AnimationOption(index));
        break;
    case 3:
        ui->chartViewPie->chart()->setAnimationOptions(QChart::AnimationOption(index));
        break;
    default:
        break;
    }
}

