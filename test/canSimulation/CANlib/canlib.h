#ifndef CANLIB_H
#define CANLIB_H

#include <QtCore/QObject>
#include "webServer.h"
#include "server.h"
#include "FileSystem.h"

class CANlib : public QObject
{
    Q_OBJECT
public:

    CANlib();

    ~CANlib();

    bool init();

public slots:

    void createConectionTCP(const QString &IP, int port);

    void writeToTCP(const QString &message);

    void createConectionWS(const QString &IP, int port);

    void writeToWS(const QString &message);

    void writeToFS(const QString &path, const QString &message);

    void readFromTCP(const QString &qMessage);

    void readFromWS(const QString &message);

    void log(const QString &qMessage, int color);


signals:

    void internal_createConectionTCP(const QString &IP, int port);

    void internal_writeToTCP(const QString &message);

    void internal_createConectionWS(const QString &IP, int port);

    void internal_writeToWS(const QString &message);

    void internal_writeToFS(const QString &path, const QString &message);


    void incomingMesageWS(const QString &qMessage);

    void incomingMesageTCP(const QString &qMessage);

    void loger(const QString &qMessage, int color);

private:
    WebServer webServer;
    Server tcpServer;
    FileSystem fileSystem;
};

#endif // CANLIB_H
