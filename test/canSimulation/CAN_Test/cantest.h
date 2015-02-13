#ifndef CANTEST_H
#define CANTEST_H

#include <QtCore/QString>
#include <QtTest/QtTest>

#include <iostream>

#include <QTcpSocket>
#include <QTcpServer>

#include "canlib.h"

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

public slots:
    void connectServer();

private:
    QTcpSocket *qTestServerSocket;
    QTcpSocket *qTestClientSocket;
    QTcpServer *qTestServer;
};

#endif // CANTEST_H
