#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSoundEffect>
#include <QPaintEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

private:
    QSoundEffect *player1;
    QSoundEffect *player2;
    QString appPath;
    QPixmap pixBackground;

    void defense(QString weapon);
    void attack(QString weapon);
    void paintEvent(QPaintEvent *event);

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_btn_DAK47_clicked();

    void on_btn_DMachinegun_clicked();

    void on_btn_DFire_clicked();

    void on_btn_DShell_clicked();

    void on_btn_DBlast1_clicked();

    void on_btn_DBlast2_clicked();

    void on_btn_A_Ak47_clicked();

    void on_btn_A_Machinegun_clicked();

    void on_btn_A_fire2_clicked();

    void on_btn_A_Mine_clicked();

    void on_btn_A_blast_clicked();

    void on_btn_A_Tank_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
