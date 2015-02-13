#include "cantest.h"

CANTest::CANTest(QObject *parent) :
    QObject(parent)
{
}

void CANTest::connectServer()
{
    qDebug() << "connection attempt" << endl;
    qTestServerSocket = qTestServer->nextPendingConnection();
}

void CANTest::initTestCase()
{
    QString ipAddress = "127.0.0.1";
    quint16 port = 9999;

    qTestServerSocket = NULL;
    qTestServer = new QTcpServer();
    qTestServer->listen( QHostAddress(ipAddress), port );
    QVERIFY( qTestServer->isListening() );
    connect( qTestServer, SIGNAL(newConnection()), SLOT(connectServer()) );

    qTestClientSocket = new QTcpSocket();
    qTestClientSocket->connectToHost( QHostAddress(ipAddress), port );

    QVERIFY( qTestClientSocket->waitForConnected( 5000 ) );
    QVERIFY( qTestClientSocket->state() == QTcpSocket::ConnectedState );
}

void CANTest::cleanupTestCase()
{
    if (qTestClientSocket != NULL) {

        qTestClientSocket->deleteLater();
    }

    if (qTestServerSocket != NULL) {

        qTestServerSocket->deleteLater();
    }

    if (qTestServer != NULL) {

        qTestServer->deleteLater();
    }
}

void CANTest::connectionTest()
{

    bool connected = (qTestClientSocket->state() == QTcpSocket::ConnectedState);

    qDebug() << "TCP Socket connection state - " <<qTestClientSocket->state();

    QVERIFY( connected );
}

void CANTest::sendDataTest()
{

}
