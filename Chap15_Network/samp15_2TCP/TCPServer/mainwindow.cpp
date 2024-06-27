#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostInfo>
#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tcpServer=new QTcpServer(this);
    labListen=new QLabel("监听状态：");
    labListen->setMinimumWidth(150);
    ui->statusBar->addWidget(labListen);

    labSocketState=new QLabel("Socket状态");
    labSocketState->setMinimumWidth(150);
    ui->statusBar->addWidget(labSocketState);

    QString hostName=QHostInfo::localHostName();
    QHostInfo hostInfo=QHostInfo::fromName(hostName);
    QString localIP;
    foreach (const auto& item, hostInfo.addresses()) {
        if(item.protocol()==QAbstractSocket::IPv4Protocol){
            localIP=item.toString();
            ui->comboIP->addItem(localIP);
        }
    }

    connect(tcpServer, &QTcpServer::newConnection,this,&MainWindow::do_newConnection);

}

MainWindow::~MainWindow()
{
    if(tcpSocket!=nullptr && (tcpSocket->state()==QAbstractSocket::ConnectedState))
        tcpSocket->disconnectFromHost();
    if(tcpServer->isListening())
        tcpServer->close();

    delete ui;
}

void MainWindow::on_actStart_triggered()
{
    QString ip=ui->comboIP->currentText();
    quint16 port=ui->spinPort->value();
    QHostAddress address(ip);
    tcpServer->listen(address,port);

    ui->textEdit->appendPlainText("**开始监听**");
    ui->textEdit->appendPlainText("**服务器地址："+tcpServer->serverAddress().toString());
    ui->textEdit->appendPlainText("**服务器端口号："+QString::number(tcpServer->serverPort()));
    ui->actStart->setEnabled(false);
    ui->actStop->setEnabled(true);

    labListen->setText("监听状态：正在监听");
}

void MainWindow::do_newConnection()
{
    tcpSocket=tcpServer->nextPendingConnection();
    connect(tcpSocket,&QTcpSocket::connected,this,&MainWindow::do_connected);
    connect(tcpSocket, &QTcpSocket::disconnected, this, [=](){
        ui->textEdit->appendPlainText("**Client socket disconnected **");
        tcpSocket->deleteLater();
    });
    connect(tcpSocket, &QTcpSocket::readyRead, this, [=](){
        while(tcpSocket->canReadLine()){
            ui->textEdit->appendPlainText("[in]"+tcpSocket->readLine());
        }

    });
    connect(tcpSocket, &QTcpSocket::stateChanged, this, [=](QAbstractSocket::SocketState socketState){
        switch(socketState)
        {
        case QAbstractSocket::UnconnectedState:
            labSocketState->setText("socket状态：UnconnectedState");
            break;
        case QAbstractSocket::HostLookupState:
            labSocketState->setText("socket状态：HostLookupState");
            break;
        case QAbstractSocket::ConnectingState:
            labSocketState->setText("socket状态：ConnectingState");
            break;
        case QAbstractSocket::ConnectedState:
            labSocketState->setText("socket状态：ConnectedState");
            break;
        case QAbstractSocket::BoundState:
            labSocketState->setText("socket状态：BoundState");
            break;
        case QAbstractSocket::ClosingState:
            labSocketState->setText("socket状态：ClosingState");
            break;
        case QAbstractSocket::ListeningState:
            labSocketState->setText("socket状态：ListeningState");
        }
    });
}

void MainWindow::do_connected()
{
    ui->textEdit->appendPlainText("**Client socket connected **");
    ui->textEdit->appendPlainText("peer address: "+tcpSocket->peerAddress().toString());
    ui->textEdit->appendPlainText("**peer port: "+QString::number(tcpSocket->peerPort()));
}


void MainWindow::on_actStop_triggered()
{
    if(tcpServer->isListening()){
        if(tcpSocket!=nullptr && (tcpSocket->state()==QAbstractSocket::ConnectedState))
            tcpSocket->disconnectFromHost();

        ui->actStart->setEnabled(true);
        ui->actStop->setEnabled(false);
        labListen->setText("监听状态：已停止");
    }
}


void MainWindow::on_actClear_triggered()
{
    ui->textEdit->clear();
}


void MainWindow::on_btnSend_clicked()
{
    if(tcpSocket==nullptr)
    {
        ui->textEdit->appendPlainText("tcpSocket==nullptr!");
        return;
    }

    QString msg=ui->editMsg->text();
    ui->textEdit->appendPlainText("[out] "+msg);
    ui->editMsg->clear();
    ui->editMsg->setFocus();

    QByteArray strByte=msg.toUtf8();
    strByte.append('\n');
    tcpSocket->write(strByte);

}

