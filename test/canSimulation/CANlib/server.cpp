#include <stdlib.h>
#include <sstream>
#include "server.h"
#include "color.h"
#include <unistd.h>

Server::Server() : QObject(), messageHandler(NULL), thread(NULL) {

    tcpServer = new QTcpServer(this);

    mutex = new QMutex();

    connect(tcpServer, SIGNAL(newConnection()),this, SLOT(connected()));
}

void Server::addThread(QTcpSocket *client)
{

    //TO DO - Add vector with MessageHandler and QThread Objects
    messageHandler = new MessageHandler(client, mutex, messagePull);
    thread = new QThread;


    //thread->run();

    messageHandler->moveToThread(thread);

    connect(thread, SIGNAL(started()), messageHandler, SLOT(process()));

    connect(this, SIGNAL(stopAllThreads()), messageHandler, SLOT(stop()));

    connect(messageHandler, SIGNAL(disconnect()), messageHandler, SLOT(deleteLater()));

    connect(this, SIGNAL(startAgaing()), messageHandler, SLOT(process()));

    connect(messageHandler, SIGNAL(requestFromTCP(const QString&)), this, SLOT(requestFromTCP(const QString&)));

    connect(messageHandler, SIGNAL(logMessageHandler(const QString&, int)), this, SLOT(logMessageHandler(const QString&, int)));

    emit log("Thread Started", RED);


    thread->start();

    return;
}

Server::~Server()
{
    emit  stopAllThreads();

    if (thread) {
        thread->quit();
        thread->wait();
    }

    if (messageHandler) {
        delete messageHandler;
        messageHandler = NULL;
    }

    if (mutex) {
        delete mutex;
        mutex = NULL;
    }

    tcpServer->deleteLater();
    if (tcpServer) {
        delete tcpServer;
        tcpServer = NULL;
    }
}

void Server::createConection(const QString &IP, int port)
{
    emit log("Start listening TCP:" + IP + ":" + QString::number(port), RED);

    QHostAddress hostadd(IP);

    if (!tcpServer->listen(hostadd, port)) {
        emit log("Unable to start the TCP server: " + tcpServer->errorString(), RED);
        return;
    }
}

bool Server::write(const QString &qMessage)
{

    mutex->lock();
    messagePull.push_back(qMessage);
    mutex->unlock();

    emit startAgaing(); //Emit signal for thread to send messages to TCP client
}

void Server::requestFromTCP(const QString &qMessage)
{
    emit readFromTCP(qMessage);
}

void Server::logMessageHandler(const QString &qMessage, int color)
{
    emit log(qMessage, color);
}

void Server::connected()
{
    emit log("TCP connected...", RED);

    emit newConnection();
    // New connected client is only one client TCP Server is working with
    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
    clientConnection->setParent(this);

    addThread(clientConnection);
}
