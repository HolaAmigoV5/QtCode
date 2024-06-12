#ifndef STUDENT_H
#define STUDENT_H

#include <QObject>

class Student : public QObject
{
    Q_OBJECT
public:
    explicit Student(QObject *parent = nullptr);

    // 定义槽函数
    // 返回值是void，有声明，也有实现，有参数，可以重载
    void treat();

    void treat(QString foodName);

signals:

};

#endif // STUDENT_H
