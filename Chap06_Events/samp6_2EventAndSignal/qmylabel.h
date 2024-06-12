#ifndef QMYLABEL_H
#define QMYLABEL_H

#include <QLabel>
#include <QObject>

class QMyLabel : public QLabel
{
    Q_OBJECT
public:
    QMyLabel(QWidget *parent=nullptr);

signals:
    void doubleClicked();

    // QWidget interface
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;
};





#endif // QMYLABEL_H
