#include "tdialogheaders.h"
#include "ui_tdialogheaders.h"

TDialogHeaders::TDialogHeaders(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TDialogHeaders)
{
    ui->setupUi(this);

    m_model=new QStringListModel(this);
    ui->listView->setModel(m_model);
}

TDialogHeaders::~TDialogHeaders()
{
    delete ui;
}

void TDialogHeaders::setHeaderList(QStringList &heads)
{
    m_model->setStringList(heads);
}

QStringList TDialogHeaders::headerList()
{
    return m_model->stringList();
}
