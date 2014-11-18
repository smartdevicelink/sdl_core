#include <stdlib.h>
#include <sstream>
#include "server.h"

Server::Server(QObject *rootObject) : rootView(rootObject), clientConnection(NULL), QObject() {

    tcpServer = new QTcpServer(this);

    //qDebug() << "port..." << tcpServer->serverPort() << tcpServer->serverAddress();

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

Server::~Server()
{
    delete tcpServer;
}

void Server::createConection(QString IP, int port)
{
    //qDebug() << "qDebug locale is createConection" << IP << port;

    loger("Listen:" + IP + ":" + QString::number(port));

    QHostAddress hostadd(IP);

    if (!tcpServer->listen(hostadd, port)) {
        //qDebug() << "Unable to start the server: %1." << tcpServer->errorString();
        loger("Unable to start the server: %1." + tcpServer->errorString());
        return;
    }
}

QVariant Server::loger(QString message)
{
    QVariant returnedValue;

    //qDebug() << message;

    QMetaObject::invokeMethod(rootView, "logger",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, message));
}

void Server::connected()
{

    loger("connected...");

    clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(clientConnection, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void Server::readyRead()
{

//    qDebug() << "Start reading data";

    QByteArray qb = clientConnection->readAll();

    const char *cMessage = qb.data();

    QVariant returnedValue;

    QString qMessage = QString::fromUtf8(cMessage);

    QMetaObject::invokeMethod(rootView, "incoming",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant,  qMessage));

    loger("Received..." + qMessage);

    qb = returnedValue.toString().toUtf8();

    char *ch = qb.data();

    write(ch);
}

void Server::disconnected()
{
    loger("disconnected...");

    clientConnection->deleteLater();
}

void Server::write(QString name)
{
    QByteArray qb = name.toUtf8();
    char *cName = qb.data();


    if ((clientConnection != NULL) && (clientConnection->state() == QTcpSocket::ConnectedState)){

        clientConnection->write(cName);
        loger("Send:" + name);
    } else {
        loger("Client is not connected yet...");
    }


//    qDebug() << QString::fromUtf8(cName);
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    loger("Error: " + socketError);
}
