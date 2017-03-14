#include "canlib.h"

CANlib::CANlib() : QObject() {
    //construtor
}

CANlib::~CANlib()
{
    //dtor
}

bool CANlib::init()
{

    QObject::connect(this, SIGNAL(internal_createConectionTCP(const QString&, int)),
                     &tcpServer, SLOT(createConection(const QString&, int)));
    QObject::connect(this, SIGNAL(internal_writeToTCP(const QString&)),
                     &tcpServer, SLOT(write(const QString&)));

    QObject::connect(this, SIGNAL(internal_writeToFS(const QString&, const QString&)),
                     &fileSystem, SLOT(write(const QString&, const QString&)));

    QObject::connect(this, SIGNAL(internal_createConectionWS(const QString&, int)),
                     &webServer, SLOT(createConection(const QString&, int)));
    QObject::connect(this, SIGNAL(internal_writeToWS(const QString&)),
                     &webServer, SLOT(write(const QString&)));

    QObject::connect(&tcpServer,SIGNAL(readFromTCP(const QString&)),
                     this, SLOT(readFromTCP(const QString &)));
    QObject::connect(&webServer,SIGNAL(readFromWS(const QString&)),
                     this, SLOT(readFromWS(const QString &)));

    QObject::connect(&fileSystem,SIGNAL(log(const QString&, int)),
                     this, SLOT(log(const QString&, int)));
    QObject::connect(&tcpServer,SIGNAL(log(const QString&, int)),
                     this, SLOT(log(const QString&, int)));
    QObject::connect(&webServer,SIGNAL(log(const QString&, int)),
                     this, SLOT(log(const QString&, int)));

    return true;
}

//-------------Redirect-invoced-functions-from-external-interfaces-to-responsible-components--------------

void CANlib::createConectionTCP(const QString &IP, int port)
{
    emit internal_createConectionTCP(IP, port);
}

void CANlib::writeToTCP(const QString &message)
{
    emit internal_writeToTCP(message);
}

void CANlib::createConectionWS(const QString &IP, int port)
{
    emit internal_createConectionWS(IP, port);
}

void CANlib::writeToWS(const QString &message)
{
    emit internal_writeToWS(message);
}

void CANlib::writeToFS(const QString &path, const QString &message)
{
    emit internal_writeToFS(path, message);
}

void CANlib::readFromTCP(const QString &qMessage)
{
    emit incomingMesageTCP(qMessage);
}

void CANlib::readFromWS(const QString &qMessage)
{
    emit incomingMesageWS(qMessage);
}

void CANlib::log(const QString &qMessage, int color)
{
    emit loger(qMessage, color);
}
