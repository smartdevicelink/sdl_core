#include <stdlib.h>

#include "server.h"

Server::Server(QObject *rootObject) : rootView(rootObject), QObject() {

    tcpServer = new QTcpServer(this);

    qDebug() << "port..." << tcpServer->serverPort() << tcpServer->serverAddress();

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

void Server::createConection(QString IP, int port)
{
    qDebug() << "qDebug locale is createConection" << IP << port;

    QHostAddress hostadd(IP);

    if (!tcpServer->listen(hostadd, port)) {
        qDebug() << "Fortune Server" << "Unable to start the server: %1." << tcpServer->errorString();

        return;
    }
}

void Server::connected()
{
    qDebug() << "connected...";

    clientConnection = tcpServer->nextPendingConnection();

    connect(clientConnection, SIGNAL(readyRead()),this, SLOT(readyRead()));
    connect(clientConnection, SIGNAL(disconnected()),this, SLOT(disconnected()));
}

void Server::readyRead()
{

    QVariant message = clientConnection->readAll();

    qDebug() <<  "Get Request - " << message;

    QVariant returnedValue;

    QMetaObject::invokeMethod(rootView, "incoming",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant, message));

    qDebug() << "Send Response - " << returnedValue;

    QString response = returnedValue.toString();

    const char* str = response.toStdString().c_str();

    clientConnection->write(str);
}

void Server::disconnected()
{
    qDebug() << "disconnected...";
}

void Server::write(QString name)
{
    qDebug() << "qDebug locale is response";

    const char* str = name.toStdString().c_str();

    clientConnection->write(str);
}

void Server::displayError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "qDebug locale is displayError" + socketError;
}
