#ifndef CANTEST_H
#define CANTEST_H

#include <QtTest/QtTest>
#include <QObject>

class CANTest : public QObject
{
    Q_OBJECT
public:
    explicit CANTest(QObject *parent = 0);

signals:

private slots:
    void myFirstTest();
};

#endif // CANTEST_H
