#ifndef NETWORK_H
#define NETWORK_H

#include <QtNetwork>
#include <QTcpSocket>
#include "messageHandler.h"

class Server : public QObject
{
    Q_OBJECT
    /**
     * Colog enum for loger
     */
    Q_ENUMS(Color)

public:
    /**
     * @brief Server
     * @param rootObject
     * ctor
     * To get access to QML from C++
     */
    Server();

    /**
     * @brief addThread
     * @param client
     */
    void addThread(QTcpSocket *client);

    QVector<QString> messagePull;

    /**
     * dtor
     */
    ~Server();

public slots:

    /**
     * @brief connected
     * Handler for new connected clients by TCP
     * Note: only last connetced one is active on Sever
     */
    void connected();

    /**
     * @brief createConection
     * @param IP - IP adres v4
     * @param port
     * Parameters to be used for connection to TCP Server
     */
    void createConection(const QString &IP, int port);

    bool write(const QString &qMessage);

    void requestFromTCP(const QString &qMessage);

    void logMessageHandler(const QString &qMessage, int color);


signals:

    void stopAllThreads();
    void startAgaing();

    void readFromTCP(const QString &qMessage);
    void log(const QString &qMessage, int color);

private:

    QTcpServer *tcpServer;
    QMutex *mutex;
};

#endif // NETWORK_H
