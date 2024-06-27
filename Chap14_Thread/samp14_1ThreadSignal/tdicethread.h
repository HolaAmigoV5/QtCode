#ifndef TDICETHREAD_H
#define TDICETHREAD_H

#include <QThread>

class TDiceThread : public QThread
{
    Q_OBJECT
public:
    explicit TDiceThread(QObject *parent = nullptr);

    void diceBegin();
    void dicePause();
    void StopThread();

private:
    int m_seq=0;
    int m_diceValue;
    bool m_pause;
    bool m_stop;

    // QThread interface
protected:
    virtual void run() override;

signals:
    void newValue(int seq, int diceValue);
};


#endif // TDICETHREAD_H
