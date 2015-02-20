#ifndef CANTEST_H
#define CANTEST_H

#include <QtCore/QString>
#include <QtTest>

#include <iostream>

#include <QTcpSocket>
#include <QTcpServer>

//#include "canlib.h"
#include "server.h"

class CANTest : public QObject
{
    Q_OBJECT
public:
    explicit CANTest(QObject *parent = 0);

private Q_SLOTS:
    void initTestCase();
    void connectionTest();
    void sendDataTest();
    void cleanupTestCase();

private:

};

#endif // CANTEST_H
