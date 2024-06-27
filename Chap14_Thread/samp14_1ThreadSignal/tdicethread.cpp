#include "tdicethread.h"

#include <QRandomGenerator>

TDiceThread::TDiceThread(QObject *parent)
    : QThread{parent}
{}

void TDiceThread::diceBegin()
{
    m_pause=false;
}

void TDiceThread::dicePause()
{
    m_pause=true;
}

void TDiceThread::StopThread()
{
    m_stop=true;
}

void TDiceThread::run()
{
    m_stop=false;
    m_pause=true;
    m_seq=0;
    while(!m_stop){
        if(!m_pause){
            ++m_seq;
            m_diceValue=QRandomGenerator::global()->bounded(1,7);
            emit newValue(m_seq,m_diceValue);
        }
        msleep(500);
    }
}

