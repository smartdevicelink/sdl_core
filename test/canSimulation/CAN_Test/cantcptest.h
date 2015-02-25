#ifndef CANTEST_H
#define CANTEST_H

#include <QtCore/QString>
#include <QtTest>

#include <iostream>

#include <QTcpSocket>
#include <QTcpServer>

#include "server.h"

class canTCPTest : public QObject
{
    Q_OBJECT
public:
    explicit canTCPTest(QObject *parent = 0);

private Q_SLOTS:
    void connectionTest();
    void receiveTCPDataTest();
    void sendTCPDataTest();

};

#endif // CANTEST_H
