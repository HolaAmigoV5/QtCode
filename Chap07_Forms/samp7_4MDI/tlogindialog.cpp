#include "tlogindialog.h"
#include "ui_tlogindialog.h"
#include <QMouseEvent>
#include <QCryptographicHash>
#include <QSettings>
#include <QMessageBox>

TLoginDialog::TLoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TLoginDialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);
    //setWindowFlags(Qt::FramelessWindowHint); // 无边框
    setWindowFlags(Qt::SplashScreen);   // 无边框

    QApplication::setOrganizationName("ABX-AB");
    QApplication::setApplicationName("wby_demo");
    readSettings();

}

TLoginDialog::~TLoginDialog()
{
    delete ui;
}

QString TLoginDialog::encrypt(const QString &str)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    hash.addData(str.toLocal8Bit());
    return hash.result();
}

// 读取注册表
void TLoginDialog::readSettings()
{
    QSettings settings;
    bool saved=settings.value("saved",false).toBool();
    m_user=settings.value("Username","user").toString();
    //m_pswd=encrypt("12345");
    m_pswd=settings.value("PSWD",encrypt("12345")).toString();
    if(saved){
        ui->editUser->setText(m_user);
    }

    ui->chkBoxSave->setChecked(saved);
}


// 写注册表
void TLoginDialog::writeSettings()
{
    QSettings settings;
    settings.setValue("Username", m_user);
    settings.setValue("PSWD", m_pswd);
    settings.setValue("saved",ui->chkBoxSave->isChecked());
}

void TLoginDialog::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_moving=true;
        m_lastPos=event->globalPosition().toPoint()-this->pos();
    }
    return QDialog::mousePressEvent(event);
}

void TLoginDialog::mouseReleaseEvent(QMouseEvent *event)
{
    m_moving=false;
    event->accept();
}

void TLoginDialog::mouseMoveEvent(QMouseEvent *event)
{
    QPoint eventPos=event->globalPosition().toPoint();

    qDebug()<<"eventPos.X = "<<eventPos.x()<<"; eventPos.Y = "<<eventPos.y()<<"\n";
    qDebug()<<"m_lastPos.X = "<<m_lastPos.x()<<"; m_lastPos.Y = "<<m_lastPos.y()<<"\n";
    qDebug()<<"pos.X = "<<pos().x()<<"; pos.Y = "<<pos().y()<<"\n";

    QPoint res=eventPos-m_lastPos-pos();
    qDebug()<<"eventPos-m_lastPos-pos() 的差值 res.X="<<res.x()<<"; res.Y="<<res.y();

    if(m_moving && (event->buttons()&Qt::LeftButton) &&
        res.manhattanLength()>QApplication::startDragDistance()){
        move(eventPos-m_lastPos);
        m_lastPos=eventPos-pos();
    }
    return QDialog::mouseMoveEvent(event);
}


void TLoginDialog::on_btnOK_clicked()
{
    QString user=ui->editUser->text().trimmed();
    QString pswd=ui->editPSWD->text().trimmed();
    QString encryptPSWD=encrypt(pswd);
    if(user==m_user && encryptPSWD==m_pswd){
        writeSettings();
        this->accept();
    }
    else{
        m_tryCount++;
        if(m_tryCount>=M_MAXCOUNT){
            QMessageBox::critical(this, "错误","密码错误次数过多，请5分钟后重试！");
            this->reject();
        }
        else{
            QMessageBox::warning(this,"提示","用户名或密码错误！");
        }
    }
}

