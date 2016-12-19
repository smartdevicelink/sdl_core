#ifndef CANWSTEST_H
#define CANWSTEST_H

#include <QtTest>
#include <QObject>
#include <QtWebSockets/QWebSocket>

#include "webServer.h"

class canWSTest : public QObject
{
    Q_OBJECT
public:
    explicit canWSTest(QObject *parent = 0);

private Q_SLOTS:
    void connectionTest();
    void receiveWSDataTest();
    void sendTCPDataTest();


private:
};

#endif // CANWSTEST_H
