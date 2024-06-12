#include "tformdoc.h"
#include "ui_tformdoc.h"
#include <QFile>
#include <QFileInfo>
#include <QFontDialog>

TFormDoc::TFormDoc(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TFormDoc)
{
    ui->setupUi(this);

    setWindowTitle("New Doc[*]");
    setAttribute(Qt::WA_DeleteOnClose);
    connect(ui->plainTextEdit,&QPlainTextEdit::modificationChanged,this, &TFormDoc::setWindowModified);
}

TFormDoc::~TFormDoc()
{
    delete ui;
}

void TFormDoc::loadFromFile(QString &aFileName)
{
    ui->plainTextEdit->clear();
    QFile aFile(aFileName);
    if(aFile.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream aStream(&aFile);
        while(!aStream.atEnd()){
            ui->plainTextEdit->appendPlainText(aStream.readLine());
        }
        aFile.close();

        QFileInfo fileInfo(aFileName);
        QString str=fileInfo.fileName();
        setWindowTitle(str+"[*]");

        m_fileName=str;
        m_fileOpened=true;
    }
}

QString TFormDoc::currentFileName()
{
    return m_fileName;
}

bool TFormDoc::isFileOpened()
{
    return m_fileOpened;
}

void TFormDoc::saveToFile()
{
    setWindowModified(false);
}

void TFormDoc::setEditFont()
{
    QFont font;
    font=ui->plainTextEdit->font();
    bool OK;
    font=QFontDialog::getFont(&OK, font);
    if(OK){
        ui->plainTextEdit->setFont(font);
    }
}

void TFormDoc::textCut()
{
    ui->plainTextEdit->cut();
}

void TFormDoc::textCopy()
{
    ui->plainTextEdit->copy();
}

void TFormDoc::textPaste()
{
    ui->plainTextEdit->paste();
}
