#include "dialog.h"
#include "ui_dialog.h"
#include <QFile>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

// 打开文件
void Dialog::on_btnOpenFile_clicked()
{
    QString curDir=QCoreApplication::applicationDirPath();
    QString aFile=QFileDialog::getOpenFileName(this,"打开一个文件",curDir,"C文件(*.h *.cpp);;文本文件(*.txt);;所有文件(*.*)");
    ui->editFile->setText(aFile);
}

// 打开目录
void Dialog::on_btnOpenDir_clicked()
{
    QString curDir=QCoreApplication::applicationDirPath();
    QString aDir=QFileDialog::getExistingDirectory(this,"选择目录",curDir);
    ui->editDir->setText(aDir);
}


void Dialog::on_btnClear_clicked()
{
    ui->plainTextEdit->clear();
}


// QCoreApplication::applicationDirPath(), applicationFilePath(), applicationName(), setApplicationName(), libraryPaths(), organizationName(), exit 退出
void Dialog::on_btnApplication_clicked()
{
    QString curDir=QCoreApplication::applicationDirPath();
    ui->plainTextEdit->appendPlainText("applicationDirPath: "+curDir);

    ui->plainTextEdit->appendPlainText("");

    QString curFile=QCoreApplication::applicationFilePath();
    ui->plainTextEdit->appendPlainText("applicationFilePath: "+curFile);

    ui->plainTextEdit->appendPlainText("");

    QString aName=QCoreApplication::applicationName();
    ui->plainTextEdit->appendPlainText("applicationName: "+aName);

    ui->plainTextEdit->appendPlainText("");

    QCoreApplication::setApplicationName("我的Demo");
    QString newName=QCoreApplication::applicationName();
    ui->plainTextEdit->appendPlainText("New applicationName: "+newName);

    ui->plainTextEdit->appendPlainText("");

    QStringList strList= QCoreApplication::libraryPaths();
    for(QString &str:strList)
        ui->plainTextEdit->appendPlainText("libraryPaths: "+str);

    ui->plainTextEdit->appendPlainText("");

    QCoreApplication::setOrganizationName("wby");
    QString orgName= QCoreApplication::organizationName();
    ui->plainTextEdit->appendPlainText("organizationName: "+orgName);
}

// QFile start
void Dialog::on_btnCopy_clicked()
{
    QString sous=ui->editFile->text();
    QFileInfo fileInfo(sous);
    QString newFile=fileInfo.path()+"/"+fileInfo.baseName()+"--副本."+fileInfo.suffix();
    QFile::copy(sous,newFile);
    ui->plainTextEdit->appendPlainText("源文件："+sous+"\r\n, 副本新文件："+newFile);
}


void Dialog::on_btnMemberCopy_clicked()
{
    QString sous=ui->editFile->text();
    QFileInfo fileInfo(sous);
    QString newFile=fileInfo.path()+"/"+fileInfo.baseName()+"--副本."+fileInfo.suffix();
    // *****************
    QFile file(sous);
    file.copy(newFile);
    // *****************
    ui->plainTextEdit->appendPlainText("源文件："+sous+"\r\n, 副本新文件："+newFile);
}


void Dialog::on_btnExists_clicked()
{
    QString sous=ui->editFile->text();
    bool the=QFile::exists(sous);
    if(the)
        ui->plainTextEdit->appendPlainText("存在文件："+sous);
    else
        ui->plainTextEdit->appendPlainText("不存在文件："+sous);
}


void Dialog::on_btnMemberExists_clicked()
{
    QString sous=ui->editFile->text();
    QFile file(sous);
    bool the=file.exists();
    if(the)
        ui->plainTextEdit->appendPlainText("存在文件："+sous);
    else
        ui->plainTextEdit->appendPlainText("不存在文件："+sous);
}


void Dialog::on_btnRemove_clicked()
{
    QString sous=ui->editFile->text();
    if(QFile::exists(sous)){
        bool res= QFile::remove(sous);
        ui->plainTextEdit->appendPlainText("删除文件："+sous+ (res==true?" 成功":" 失败"));
    }
    else
        ui->plainTextEdit->appendPlainText("文件不存在");
}


void Dialog::on_btnMemRemove_clicked()
{
    QString sous=ui->editFile->text();
    QFile file(sous);
    if(file.exists()){
        bool res= file.remove();
        ui->plainTextEdit->appendPlainText("删除文件："+sous+ (res==true?" 成功":" 失败"));
    }
    else {
        ui->plainTextEdit->appendPlainText("文件不存在");
    }
}


void Dialog::on_btnRename_clicked()
{
    QString sous=ui->editFile->text();
    QFileInfo fileInfo(sous);
    QString newFile=fileInfo.path()+"/"+fileInfo.baseName()+"--修改后版本."+fileInfo.suffix();
    QFile::rename(sous,newFile);
    ui->plainTextEdit->appendPlainText("源文件："+sous +", \n修改后文件："+newFile);
}


void Dialog::on_btnMemRename_clicked()
{
    QString sous=ui->editFile->text();
    QFileInfo fileInfo(sous);
    QString newFile=fileInfo.path()+"/"+fileInfo.baseName()+"--修改后版本."+fileInfo.suffix();
    QFile file(sous);
    file.rename(newFile);
    ui->plainTextEdit->appendPlainText("源文件："+sous +", \n修改后文件："+newFile);
}


void Dialog::on_btnMoveToTrash_clicked()
{
    QString sous=ui->editFile->text();
    if(QFile::exists(sous)){
        bool res= QFile::moveToTrash(sous);
        ui->plainTextEdit->appendPlainText("移动到回收站："+sous+ (res==true?" 成功":" 失败"));
    }
    else
        ui->plainTextEdit->appendPlainText("文件不存在。");
}


void Dialog::on_btnMemMoveToTrash_clicked()
{
    QString sous=ui->editFile->text();
    QFile file(sous);
    if(file.exists()){
        bool res= file.moveToTrash(sous);
        ui->plainTextEdit->appendPlainText("移动到回收站："+sous+ (res==true?" 成功":" 失败"));
    }
    else
        ui->plainTextEdit->appendPlainText("文件不存在。");
}

QString bool2QString(bool value){
    return value==true?"是":"不是";
}

// QFileInfo start
void Dialog::on_btnAbsoluteFilePath_clicked()
{
    QString curFile=ui->editFile->text().trimmed();
    QFileInfo fileinfo(curFile);
    QString aFilePath= fileinfo.absoluteFilePath();
    QString aPath=fileinfo.absolutePath();
    ui->plainTextEdit->appendPlainText("当前文件："+curFile+"\r\n");
    ui->plainTextEdit->appendPlainText("absoluteFilePath: "+aFilePath+"\n absolutePath: "+aPath+"\r\n");

    QString fileName=fileinfo.fileName();
    QString filePath=fileinfo.filePath();
    ui->plainTextEdit->appendPlainText("fileName: "+fileName+"\n filePath: "+filePath +"\r\n");

    QString size=QString::number(fileinfo.size()/1024)+" KB";
    ui->plainTextEdit->appendPlainText("size: "+size+"\n path: "+fileinfo.path()+"\r\n");

    ui->plainTextEdit->appendPlainText("baseName: "+fileinfo.baseName()+"\n completeBaseName: "+fileinfo.completeBaseName()+"\r\n");

    ui->plainTextEdit->appendPlainText("suffix: "+fileinfo.suffix()+"\n completeSuffix: "+fileinfo.completeSuffix()+"\r\n");    // 前面返回.oo, 后面返回aa.oo

    ui->plainTextEdit->appendPlainText("isDir: "+ bool2QString(fileinfo.isDir())+"\n isFile: "+bool2QString(fileinfo.isFile())+"\r\n");

    ui->plainTextEdit->appendPlainText("isExecuteble: "+bool2QString(fileinfo.isExecutable())+"\n birthTime: "+fileinfo.birthTime().toString("yyyy-MM-dd hh:mm:ss")+"\r\n");

    ui->plainTextEdit->appendPlainText("lastModified: "+fileinfo.lastModified().toString("yyyy-MM-dd hh:mm:ss")+
                                       "\n lastRead: "+fileinfo.lastRead().toString("yyyy-MM-dd hh:mm:ss")+"\r\n");

    ui->plainTextEdit->appendPlainText("Static exists: "+bool2QString(QFileInfo::exists(curFile))+"\n exists: "+bool2QString(fileinfo.exists())+"\r\n");
}



// QDir start
void Dialog::on_btntempPath_clicked()
{
    ui->plainTextEdit->appendPlainText("tempPath: "+QDir::tempPath()+"\r\n");
    ui->plainTextEdit->appendPlainText("rootPath: "+QDir::rootPath()+"\r\n");
    ui->plainTextEdit->appendPlainText("homePath: "+QDir::homePath()+"\r\n");

    QFileInfoList fileInfos=QDir::drives();
    for(QFileInfo &info:fileInfos)
        ui->plainTextEdit->appendPlainText("drives: "+info.path()+"\r\n");

    ui->plainTextEdit->appendPlainText("currentPath: "+QDir::currentPath()+"\r\n");
}


void Dialog::on_btnSetCurrent_clicked()
{
    QString curPath=ui->editDir->text().trimmed();
    QDir::setCurrent(curPath);
    ui->plainTextEdit->appendPlainText("setCurrent后: "+QDir::currentPath()+"\r\n");
}




void Dialog::on_btnMkdir_clicked()
{
    QDir dir(ui->editDir->text().trimmed());
    bool ok=dir.mkdir("wby");
    ui->plainTextEdit->appendPlainText("创建目录："+bool2QString(ok) +"成功");
}


void Dialog::on_btnRmdir_clicked()
{
    QString sous=ui->editDir->text().trimmed();
    QDir dir(sous);
    bool ok=dir.rmdir(sous);
    ui->plainTextEdit->appendPlainText("删除目录："+bool2QString(ok) +"成功");
}


void Dialog::on_btnQDirRemove_clicked()
{
    QDir dir(ui->editDir->text().trimmed());
    bool ok=dir.remove(ui->editFile->text());
    ui->plainTextEdit->appendPlainText("删除文件："+bool2QString(ok) +"成功");
}


void Dialog::on_btnQDirRename_clicked()
{
    QDir dir(ui->editDir->text().trimmed());
    QString sous=ui->editFile->text();
    QFileInfo fileInfo(sous);
    QString newFile=fileInfo.path()+"/"+fileInfo.baseName()+".abc";
    bool ok=dir.rename(sous,newFile);
    ui->plainTextEdit->appendPlainText("重命名文件："+bool2QString(ok) +"成功");
}


void Dialog::on_btnSetPath_clicked()
{
    QString curDir=QDir::currentPath();
    QDir lastDir(curDir);
    lastDir.setPath(ui->editDir->text());
    ui->plainTextEdit->appendPlainText("setPath前："+curDir+" \n setPath后："+lastDir.absolutePath());
}


void Dialog::on_btnRemoveRecur_clicked()
{
    QDir dir(ui->editDir->text());
    bool ok=dir.removeRecursively();
    ui->plainTextEdit->appendPlainText("递归删除目录"+bool2QString(ok) +"成功");
}


void Dialog::on_btnAbsPath_clicked()
{
    QDir dir(ui->editDir->text());
    QString str=dir.absoluteFilePath(ui->editFile->text());
    ui->plainTextEdit->appendPlainText("absoluteFilePath: "+str+"\r\n");

    QString absPath=dir.absolutePath();
    ui->plainTextEdit->appendPlainText("absolutePath: "+absPath+"\r\n");

    QString canPath=dir.canonicalPath();
    ui->plainTextEdit->appendPlainText("canonicalPath: "+canPath+"\r\n");

    QString fPath=dir.filePath(ui->editFile->text());
    ui->plainTextEdit->appendPlainText("filePath: "+fPath+"\r\n");

    QString path=dir.path();
    ui->plainTextEdit->appendPlainText("path: "+path+"\r\n");

    QString dirName=dir.dirName();
    ui->plainTextEdit->appendPlainText("dirName: "+dirName+"\r\n");

    bool dirExists=dir.exists();
    ui->plainTextEdit->appendPlainText("文件: "+bool2QString(dirExists)+"存在"+"\r\n");

    bool isEmpty=dir.isEmpty();
    ui->plainTextEdit->appendPlainText("目录 "+bool2QString(isEmpty)+"空的"+"\r\n");

    // 当前目录下的子目录
    QStringList strList=dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    ui->plainTextEdit->appendPlainText("所选目录下的所有子目录：");
    for(QString &str:strList)
        ui->plainTextEdit->appendPlainText(str);

    ui->plainTextEdit->appendPlainText("\r\n");

    // 当前目录下的文件
    QStringList files=dir.entryList(QDir::Files);
    ui->plainTextEdit->appendPlainText("所选目录下的所有文件：");
    for(QString &item:files)
        ui->plainTextEdit->appendPlainText(item);

}

// 创建临时目录
#include <QTemporaryDir>
void Dialog::on_btnMkTempDir_clicked()
{
    ui->plainTextEdit->appendPlainText("QDir::tempPath()= "+QDir::tempPath());
    QTemporaryDir dir;
    dir.setAutoRemove(true);
    ui->plainTextEdit->appendPlainText("系统临时目录中创建："+dir.path()+"\n");
}



#include <QTemporaryFile>
void Dialog::on_btnMkTempFile_clicked()
{
    ui->plainTextEdit->appendPlainText("QDir::tempPath()= "+QDir::tempPath());
    QTemporaryFile file;
    file.setAutoRemove(true);
    file.open();
    ui->plainTextEdit->appendPlainText("系统临时目录中创建："+file.fileName()+"\n");
    file.close();
}

void Dialog::on_btnMkDir_clicked()
{
    QString specDir=ui->editDir->text();
    ui->plainTextEdit->appendPlainText("指定的目录："+specDir);

    QTemporaryDir dir(specDir +"/tempDir");
    dir.setAutoRemove(false);
    ui->plainTextEdit->appendPlainText("系统指定目录中创建："+dir.path());
}


void Dialog::on_btnMkFile_clicked()
{
    QString specDir=ui->editDir->text();
    ui->plainTextEdit->appendPlainText("指定的目录："+specDir);

    QTemporaryFile aFile(specDir +"/xxx.temp");
    aFile.setAutoRemove(false);
    aFile.open();
    ui->plainTextEdit->appendPlainText("系统指定目录中创建："+aFile.fileName());
    aFile.close();
}


void Dialog::on_btnMkDirInCurrent_clicked()
{
    ui->plainTextEdit->appendPlainText("当前的目录："+QDir::currentPath());

    QTemporaryDir dir("tempDir");
    dir.setAutoRemove(false);
    ui->plainTextEdit->appendPlainText("当前目录中创建："+dir.path());
}


void Dialog::on_btnMkFileInCurrent_clicked()
{
    ui->plainTextEdit->appendPlainText("当前的目录："+QDir::currentPath());

    QTemporaryFile aFile("xxx.temp");
    aFile.setAutoRemove(false);
    aFile.open();
    ui->plainTextEdit->appendPlainText("当前的目录中创建："+aFile.fileName());
    aFile.close();
}

// QFileSystemWatcher start

void Dialog::on_btnAddPath_clicked()
{
    fileWatcher->addPath(ui->editDir->text());
    fileWatcher->addPath(ui->editFile->text());

    connect(fileWatcher,&QFileSystemWatcher::directoryChanged, this, &Dialog::do_directoryChanged);
    connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, &Dialog::do_fileChanged);
}

void Dialog::do_directoryChanged(const QString &path)
{
    ui->plainTextEdit->appendPlainText(path + " 目录发生了变化");
}

void Dialog::do_fileChanged(const QString &path)
{
    ui->plainTextEdit->appendPlainText(path + " 文件发生了变化");
}


void Dialog::on_btnRemovePath_clicked()
{
    fileWatcher->removePath(ui->editDir->text());
    fileWatcher->removePath(ui->editFile->text());
    disconnect(fileWatcher);
}

// 列出所有监听的目录
void Dialog::on_btnDirectory_clicked()
{
    for(auto &item:fileWatcher->directories())
        ui->plainTextEdit->appendPlainText("监听的目录有："+item);

    ui->plainTextEdit->appendPlainText("");
}

// 列出所有监听的文件
void Dialog::on_btnFiles_clicked()
{
    for(auto &item:fileWatcher->files())
        ui->plainTextEdit->appendPlainText("监听的文件有"+item);

    ui->plainTextEdit->appendPlainText("");
}

