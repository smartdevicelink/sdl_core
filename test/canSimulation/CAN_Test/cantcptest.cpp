#include "cantcptest.h"

canTCPTest::canTCPTest(QObject *parent) :
    QObject(parent)
{
}

void canTCPTest::connectionTest()
{
     Server server;

     QString ipAddress = "127.0.0.1";
     quint16 port = 6666;
     server.createConection(ipAddress, port );

     QTcpSocket s;
     QSignalSpy spy(&server, SIGNAL(newConnection()));
     s.connectToHost(QHostAddress(ipAddress), port );

     QCOMPARE(spy.isValid(), true);

     QTRY_COMPARE_WITH_TIMEOUT(spy.count(),1, 1000);
     s.close();
}

void canTCPTest::receiveTCPDataTest()
{
    QString qMessage = "Hello";

    Server server;

    QString ipAddress = "127.0.0.1";
    quint16 port = 6667;
    server.createConection(ipAddress, port );

    QTcpSocket s;
    QSignalSpy spy(&server, SIGNAL(readFromTCP(const QString &)));
    s.connectToHost(QHostAddress(ipAddress), port );

    QCOMPARE(spy.isValid(), true); // Returns true if the signal spy listens to a valid signal, otherwise false.

    QVERIFY2(s.waitForConnected( 5000 ), "TCP Client can not connect!");

    QVERIFY2(s.write(qMessage.toUtf8()) != -1, "Data can not be writen!");
    QVERIFY2(s.flush(), "Data was not send!");

    QTRY_COMPARE_WITH_TIMEOUT(spy.count(),1, 2000); // make sure the signal was emitted exactly one time

    QList<QVariant> arguments = spy.takeFirst(); // take the first signal

    QVERIFY2(qMessage == arguments.at(0).toString(), "Received message on server is not valid!"); // verify the first argument

    s.close();
    s.deleteLater();
    server.deleteLater();

}

void canTCPTest::sendTCPDataTest()
{
    QString qMessage = "Hello";

    Server server;

    QString ipAddress = "127.0.0.1";
    quint16 port = 6669;
    server.createConection(ipAddress, port );

    QTcpSocket s;
    s.connectToHost(QHostAddress(ipAddress), port );

    QVERIFY2(s.waitForConnected( 5000 ), "TCP Client can not connect!");

    QTest::qWait(1000);

    server.write(qMessage);

    QVERIFY(s.waitForReadyRead(5000));
    QByteArray arr = s.readAll();
    QCOMPARE(arr.constData(), qMessage.toUtf8().data());

    s.close();
    s.deleteLater();
    server.deleteLater();
}
