#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileSystemWatcher>

QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_btnOpenFile_clicked();

    void on_btnOpenDir_clicked();

    void on_btnClear_clicked();

    void on_btnApplication_clicked();


    void on_btnCopy_clicked();

    void on_btnExists_clicked();

    void on_btnMemberCopy_clicked();

    void on_btnMemberExists_clicked();

    void on_btnRemove_clicked();

    void on_btnMemRemove_clicked();

    void on_btnRename_clicked();

    void on_btnMemRename_clicked();

    void on_btnMoveToTrash_clicked();

    void on_btnMemMoveToTrash_clicked();

    void on_btnAbsoluteFilePath_clicked();

    void on_btntempPath_clicked();

    void on_btnSetCurrent_clicked();

    void on_btnMkdir_clicked();

    void on_btnRmdir_clicked();

    void on_btnQDirRemove_clicked();

    void on_btnQDirRename_clicked();

    void on_btnSetPath_clicked();

    void on_btnRemoveRecur_clicked();

    void on_btnAbsPath_clicked();

    void on_btnMkTempDir_clicked();

    void on_btnMkDir_clicked();

    void on_btnMkTempFile_clicked();

    void on_btnMkFile_clicked();

    void on_btnMkDirInCurrent_clicked();

    void on_btnMkFileInCurrent_clicked();

    void on_btnAddPath_clicked();

private slots:
    void do_directoryChanged(const QString &path);
    void do_fileChanged(const QString &path);
    void on_btnRemovePath_clicked();

    void on_btnDirectory_clicked();

    void on_btnFiles_clicked();

private:
    Ui::Dialog *ui;

    QFileSystemWatcher *fileWatcher =new QFileSystemWatcher(this);
};
#endif // DIALOG_H
