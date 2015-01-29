#include <stdlib.h>
#include <sstream>
#include "server.h"
#include "loger.h"
#include "color.h"
#include <unistd.h>

Server::Server(QObject *rootObject) : rootView(rootObject), QObject() {

    tcpServer = new QTcpServer(this);

    mutex = new QMutex();

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

void Server::addThread(QTcpSocket *client)
{
    MessageHandler* messageHandler = new MessageHandler(rootView, client, mutex, messagePull);
    QThread* thread = new QThread;


    //thread->run();

    messageHandler->moveToThread(thread);

    connect(thread, SIGNAL(started()), messageHandler, SLOT(process()));

    connect(this, SIGNAL(stopAllThreads()), messageHandler, SLOT(stop()));

    connect(messageHandler, SIGNAL(disconnect()), messageHandler, SLOT(deleteLater()));

    connect(this, SIGNAL(startAgaing()), messageHandler, SLOT(process()));

    connect(messageHandler, SIGNAL(requestFromTCP(const QString&)), this, SLOT(requestFromTCP(const QString&)));

    connect(messageHandler, SIGNAL(log(const QString&, int)), this, SLOT(log(const QString&, int)));

    Loger::loger("Thread Started", RED);


    thread->start();

    return;
}

Server::~Server()
{
    emit  stopAllThreads();
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

void Server::write(const QString &message)
{

    mutex->lock();
    messagePull.push_back(message);
    qDebug() << messagePull.count();
    mutex->unlock();
    emit startAgaing();
}

void Server::requestFromTCP(const QString &qMessage)
{
    QVariant returnedValue;

    QMetaObject::invokeMethod(rootView, "incoming",
            Q_RETURN_ARG(QVariant, returnedValue),
            Q_ARG(QVariant,  qMessage));

    // If processed data type was request
    // than response from QML is sent back to client

    if (returnedValue != "false") {

        QString message = returnedValue.toString();

        write(message);
    }
}

void Server::log(const QString &qMessage, int color)
{
    Loger::loger(qMessage, color);
}

void Server::connected()
{

    Loger::loger("TCP connected...", RED);

    // New connected client is only one client TCP Server is working with
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    clientConnection->setParent(this);

    addThread(clientConnection);
}
