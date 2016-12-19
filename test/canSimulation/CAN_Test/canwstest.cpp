#include "canwstest.h"
#include "QtWebSockets/QWebSocket"

canWSTest::canWSTest(QObject *parent) :
    QObject(parent)
{
}

void canWSTest::connectionTest()
{
    WebServer server;

    QString ipAddress = "127.0.0.1";
    quint16 port = 6666;
    server.createConection(ipAddress, port );

    QSignalSpy spy(&server, SIGNAL(newConnection()));

    QCOMPARE(spy.isValid(), true);


    QUrl _url = QStringLiteral("ws://localhost:6666");

    QWebSocket _webSocket;
    _webSocket.open(_url);

    QTRY_COMPARE_WITH_TIMEOUT(spy.count(), 1, 3000);

    _webSocket.close();
}

void canWSTest::receiveWSDataTest()
{
    QString qMessage = "Hello";

    WebServer server;

    QString ipAddress = "127.0.0.1";
    quint16 port = 6666;
    server.createConection(ipAddress, port );

    QSignalSpy spy(&server, SIGNAL(readFromWS(const QString &)));

    QCOMPARE(spy.isValid(), true);


    QUrl _url = QStringLiteral("ws://localhost:6666");

    QWebSocket _webSocket;
    _webSocket.open(_url);

    QCOMPARE(spy.isValid(), true); // Returns true if the signal spy listens to a valid signal, otherwise false.

    QTest::qWait(1000);

    QVERIFY2(_webSocket.sendTextMessage(qMessage) != -1, "Data can not be writen!");

    QTRY_COMPARE_WITH_TIMEOUT(spy.count(),1, 2000); // make sure the signal was emitted exactly one time

    QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    QVERIFY2(qMessage == arguments.at(0).toString(), "Received message on server is not valid!"); // verify the first argument

    _webSocket.close();

}

void canWSTest::sendTCPDataTest()
{
    QString qMessage = "Hello";

    WebServer server;

    QString ipAddress = "127.0.0.1";
    quint16 port = 6666;
    server.createConection(ipAddress, port );

    QUrl _url = QStringLiteral("ws://localhost:6666");

    QWebSocket _webSocket;
    _webSocket.open(_url);

    QSignalSpy spy(&_webSocket, SIGNAL(textMessageReceived(QString)));

    QTest::qWait(1000);

    server.write(qMessage);

    QTRY_COMPARE_WITH_TIMEOUT(spy.count(),1, 2000); // make sure the signal was emitted exactly one time

    QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    QVERIFY2(qMessage == arguments.at(0).toString(), "Received message on server is not valid!"); // verify the first argument

    _webSocket.close();
}
