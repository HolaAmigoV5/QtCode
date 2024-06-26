#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_btnFontBack_clicked();

    void on_btnLeftRight_clicked();

    void on_btnClear_clicked();

    void on_btnFirstLast_clicked();

    void on_btnSection_clicked();

    void on_btnisNullEmpty_clicked();

    void on_btnResize_clicked();

    void on_btnSizeLength_clicked();

    void on_btnFill_clicked();

    void on_btnTrimmed_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
