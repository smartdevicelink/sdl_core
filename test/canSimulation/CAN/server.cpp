#include <stdlib.h>
#include <sstream>
#include "server.h"
#include "loger.h"
#include "color.h"

Server::Server(QObject *rootObject) : rootView(rootObject), clientConnection(NULL), QObject() {

    tcpServer = new QTcpServer(this);

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

Server::~Server()
{
    delete tcpServer;
}

void Server::createConection(QString IP, int port)
{
    Loger::loger("Start listening TCP:" + IP + ":" + QString::number(port), RED);

    QHostAddress hostadd(IP);

    if (!tcpServer->listen(hostadd, port)) {
        Loger::loger("Unable to start the TCP server: " + tcpServer->errorString(), RED);
        return;
    }
}

void Server::connected()
{

    Loger::loger("TCP connected...", RED);

    clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(clientConnection, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void Server::readyRead()
{

    QByteArray qb = clientConnection->readAll();

    const char *cMessage = qb.data();

    QVariant returnedValue;

    QString qMessage = QString::fromUtf8(cMessage);

    QMetaObject::invokeMethod(rootView, "incoming",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant,  qMessage));

    Loger::loger("TCP Received..." + qMessage, GREEN);

    qb = returnedValue.toString().toUtf8();

    char *ch = qb.data();

    write(ch);
}

void Server::disconnected()
{
    Loger::loger("TCP disconnected...", RED);

    clientConnection->deleteLater();
}

void Server::write(QString name)
{
    QByteArray qb = name.toUtf8();

    if ((clientConnection != NULL) && (clientConnection->state() == QTcpSocket::ConnectedState)){

        clientConnection->write(qb);
        clientConnection->flush();
        Loger::loger("TCP Send:" + name, BLUE);
    } else {
        Loger::loger("TCP Client is not connected yet...", RED);
    }
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    Loger::loger("TCP Error: " + socketError, RED);
}
