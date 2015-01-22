#include <stdlib.h>
#include <sstream>
#include "server.h"
#include "loger.h"
#include "color.h"
#include <unistd.h>

Server::Server(QObject *rootObject) : rootView(rootObject), clientConnection(NULL), QObject() {

    tcpServer = new QTcpServer(this);

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

Server::~Server()
{
    delete tcpServer;
}

void Server::createConection(const QString &IP, int port)
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

    // New connected client is only one client TCP Server is working with
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

    Loger::loger("TCP Received..." + qMessage, GREEN);

    // Read all data came from client and redirect it to QML request handler

    QMetaObject::invokeMethod(rootView, "incoming",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant,  qMessage));

    // If processed data type was request
    // than response from QML is sent back to client

    if (returnedValue != "false") {

        qb = returnedValue.toString().toUtf8();

        char *ch = qb.data();

        QString message = QString::fromUtf8(ch);

        write(message);
    }
}

void Server::disconnected()
{
    Loger::loger("TCP disconnected...", RED);

    clientConnection->deleteLater();
}

void Server::write(const QString &qMessage)
{
    QByteArray qb = qMessage.toUtf8();

    if ((clientConnection != NULL) && (clientConnection->state() == QTcpSocket::ConnectedState)){

        clientConnection->write(qb);
        clientConnection->flush();
        Loger::loger("TCP Send:" + qMessage, BLUE);
    } else {
        Loger::loger("TCP Client is not connected yet...", RED);
    }
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    Loger::loger("TCP Error: " + socketError, RED);
}
