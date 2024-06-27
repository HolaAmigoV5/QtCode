#include "mainwindow.h"
#include "ui_mainwindow.h"

QString MainWindow::protocolName(QAbstractSocket::NetworkLayerProtocol protocol)
{
    switch (protocol) {
    case QAbstractSocket::IPv4Protocol:
        return "IPv4";
    case QAbstractSocket::IPv6Protocol:
        return "IPv6";
    default:
        return "UnKnown NetWork Layer Protocol";
    }
}

QString MainWindow::interfaceType(QNetworkInterface::InterfaceType type)
{
    switch (type) {
    case QNetworkInterface::Unknown:
        return "UnKnown";
    case QNetworkInterface::Loopback:
        return "Loopback";
    case QNetworkInterface::Ethernet:
        return "Ethernet";
    case QNetworkInterface::Wifi:
        return "Wifi";
    default:
        return "Other Type";
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnGetHostInfo_clicked()
{
    ui->textEdit->clear();
    QString hostName=QHostInfo::localHostName();
    ui->textEdit->appendPlainText("主机名称为："+ hostName +"\n");

    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QList<QHostAddress> addrList=hostInfo.addresses();
    if(addrList.isEmpty())
        return;

    foreach (const auto &host, addrList) {
        bool show=ui->chkBox_OnlyIPv4->isChecked();
        if(show && host.protocol()!=QAbstractSocket::IPv4Protocol){
            continue;
        }

        ui->textEdit->appendPlainText("协议："+protocolName(host.protocol()));
        ui->textEdit->appendPlainText(host.toString());
        ui->textEdit->appendPlainText(QString("isGlobal = %1\n").arg(host.isGlobal()));
    }
}


void MainWindow::on_btnLookup_clicked()
{
    ui->textEdit->clear();
    QString hostName=ui->comboBox->currentText();
    ui->textEdit->appendPlainText("正在查询的主机信息是："+hostName);

    // QList<QHostAddress> addrList= QHostInfo::fromName(hostName).addresses();
    // if(addrList.isEmpty())
    //     return;

    // foreach (const auto &host, addrList) {
    //     bool show=ui->chkBox_OnlyIPv4->isChecked();
    //     if(show && host.protocol()!=QAbstractSocket::IPv4Protocol){
    //         continue;
    //     }

    //     ui->textEdit->appendPlainText("协议："+protocolName(host.protocol()));
    //     ui->textEdit->appendPlainText(host.toString());
    //     ui->textEdit->appendPlainText(QString("isGlobal = %1\n").arg(host.isGlobal()));
    // }

    QHostInfo::lookupHost(hostName,this, SLOT(do_lookedUpHostInfo(QHostInfo)));
}

void MainWindow::do_lookedUpHostInfo(const QHostInfo &host)
{
    QList<QHostAddress> addrList=host.addresses();

    foreach (const auto &host, addrList) {
        bool show=ui->chkBox_OnlyIPv4->isChecked();
        if(show && host.protocol()!=QAbstractSocket::IPv4Protocol){
            continue;
        }

        ui->textEdit->appendPlainText("协议："+protocolName(host.protocol()));
        ui->textEdit->appendPlainText(host.toString());
        ui->textEdit->appendPlainText(QString("isGlobal = %1\n").arg(host.isGlobal()));
    }
}


void MainWindow::on_btnAllAddress_clicked()
{
    ui->textEdit->clear();
    QList<QHostAddress> addrList= QNetworkInterface::allAddresses();
    if(addrList.isEmpty())
        return;

    foreach (const auto &host, addrList) {
        bool show=ui->chkBox_OnlyIPv4->isChecked();
        if(show && host.protocol()!=QAbstractSocket::IPv4Protocol){
            continue;
        }

        ui->textEdit->appendPlainText("协议："+protocolName(host.protocol()));
        ui->textEdit->appendPlainText(host.toString());
        ui->textEdit->appendPlainText(QString("isGlobal = %1\n").arg(host.isGlobal()));
    }
}


void MainWindow::on_btnAllInterface_clicked()
{
    ui->textEdit->clear();
    QList<QNetworkInterface> list= QNetworkInterface::allInterfaces();
    if(list.isEmpty())
        return;

    foreach (const auto &item, list) {
        if(!item.isValid())
            continue;

        ui->textEdit->appendPlainText("设备名称："+item.humanReadableName());
        ui->textEdit->appendPlainText("硬件地址："+item.hardwareAddress());
        ui->textEdit->appendPlainText("接口类型："+interfaceType(item.type()));

        ui->textEdit->appendPlainText("\n");
        QList<QNetworkAddressEntry> entryList=item.addressEntries();
        foreach (const auto &entry, entryList) {
            ui->textEdit->appendPlainText("IP 地址："+entry.ip().toString());
            ui->textEdit->appendPlainText("子网掩码："+entry.netmask().toString());
            ui->textEdit->appendPlainText("广播地址："+entry.broadcast().toString());
        }
        ui->textEdit->appendPlainText("\r\n");
    }
}

